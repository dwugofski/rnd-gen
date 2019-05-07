
#include "core.h"

using namespace xml;

const string node::classname = "node";

node::node() {
	_space = 0;
	_parent = 0;
}

node::node(xml_wrapper* wrapper, node* my_parent, const xml_node* base_node) : node() {
	const xml_attribute* curr_attr = base_node->first_attribute();
	while (curr_attr != 0) {
		add_attr(std::tolower(curr_attr->name(), std::locale()), curr_attr->value());
		curr_attr = curr_attr->next_attribute();
	}
	if (!has_attr(ID_STRING)) throw(NO_ID);
	else if (attr(ID_STRING).size() < MIN_ID_LENGTH) throw(NO_ID);
	else _id = _attrs[ID_STRING].value;

	_parent = my_parent;
	if (_parent != 0) my_parent->add_child(this);

	const xml_node* child_base = base_node->first_node();
	while (child_base != 0) {
		node* new_node = wrapper->create_node(this, child_base);
		child_base = child_base->next_sibling();
	}
}

node::node(const string &id) : node() {
	if (id.size() < MIN_ID_LENGTH) throw(NO_ID);
	else _id = id;
}

node::node(node* my_parent, const string& id) : node(id) {
	_parent = my_parent;
	if (_parent != 0) my_parent->add_child(this);
}

node::node(node* my_parent, const string& id, const vector<attribute> &new_attrs) : node(my_parent, id) {
	for (vector<attribute>::const_iterator it = new_attrs.begin(); it != new_attrs.end(); it++) {
		add_attr(std::tolower(it->name, std::locale()), it->value);
	}
}

node::~node() {
	for (hashmap<string, node*>::iterator it = _children.begin(); it != _children.end(); ++it) {
		delete it->second;
	}
	if (has_parent()) _parent->remove_child(this);
	if (has_space()) _space->remove_node(this);
}

bool node::instanceof(const string &class_name) const {
	if (class_name == node::classname) return true;
	return false;
}

string node::id() const {
	return _id;
}

vector<const node*> node::children() const {
	vector<const node*> ret;
	for (hashmap<string, node*>::const_iterator it = _children.begin(); it != _children.end(); ++it) {
		ret.push_back(it->second);
	}
	return ret;
}
vector<node*> node::children(){
	vector<node*> ret;
	for (hashmap<string, node*>::iterator it = _children.begin(); it != _children.end(); ++it) {
		ret.push_back(it->second);
	}
	return ret;
}

const node* node::child(const string &child_name) const {
	if (!has_child(child_name)) throw(CHILD_NOT_FOUND);
	return _children.find(child_name)->second;
}
node* node::child(const string &child_name) {
	return const_cast<node*>(static_cast<const node*>(this)->child(child_name));
}

bool node::has_child(const string &child_name) const {
	return _children.find(child_name) != _children.end();
}
bool node::has_child(const node* child_node) const {
	return has_child(child_node->id());
}

void node::add_child(node* new_child) {
	if (has_child(new_child)) return;
	new_child->parent(0);
	_children[new_child->id()] = new_child;
	new_child->parent(this);
}

void node::remove_child(node* old_child) {
	if (!has_child(old_child)) return;
	_children.erase(old_child->id());
	old_child->parent(0);
}



bool node::has_parent() const {
	return parent() != 0;
}

const node* node::parent() const {
	return _parent;
}
node* node::parent() {
	return const_cast<node*>(static_cast<const node*>(this)->parent());
}

void node::parent(node* new_parent) {
	if (new_parent == _parent) return;
	if (has_parent()) {
		node* curr_parent = _parent;
		_parent = 0;
		curr_parent->remove_child(this);
		space(0);
	}
	_parent = new_parent;
	_parent->add_child(this);
	space(_parent->child_space());
}



bool node::has_space() const {
	return _space != 0;
}

const name_space* node::space() const {
	return _space;
}
name_space* node::space() {
	return const_cast<name_space*>(static_cast<const node*>(this)->space());
}

void node::space(name_space* new_space) {
	if (new_space == _space) return;
	if (has_space()) {
		name_space* curr_space = _space;
		_space = 0;
		curr_space->remove_node(this);
	}
	_space = new_space;
	_space->add_node(this);
}

const name_space* node::child_space() const {
	return _space;
}
name_space* node::child_space() {
	return const_cast<name_space*>(static_cast<const node*>(this)->child_space());
}



vector<attribute> node::attributes() const {
	vector<attribute> ret;
	for (hashmap<string, attribute>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it) {
		ret.push_back(it->second);
	}
	return ret;
}

string node::attr(const string& attr_name) const {
	if (!has_attr(attr_name)) throw(ATTRIBUTE_NOT_FOUND);
	return _attrs.find(attr_name)->second.value;
}
void node::attr(const string& attr_name, const string& attr_val) {
	if (!has_attr(attr_name)) throw(ATTRIBUTE_NOT_FOUND);
	_attrs.find(attr_name)->second.value = attr_val;
}
void node::attr(const attribute attr_pair) {
	if (!has_attr(attr_pair.name)) throw(ATTRIBUTE_NOT_FOUND);
	_attrs.find(attr_pair.name)->second.value = attr_pair.value;
}

bool node::has_attr(const string& attr_name) const {
	return _attrs.find(attr_name) != _attrs.end();
}
bool node::has_attr(const attribute attr) const {
	return _attrs.find(attr.name) != _attrs.end();
}

void node::add_attr(const attribute new_attr) {
	if (has_attr(new_attr)) attr(new_attr);
	else {
		_attrs[new_attr.name] = new_attr;
	}
}
void node::add_attr(const string &attr_name, const string& attr_value) {
	attribute new_attr = { attr_name, attr_value };
	add_attr(new_attr);
}
void node::add_attr(const string &attr_name) {
	add_attr(attr_name, DEFAULT_VALUE);
}

void node::remove_attr(const string& attr_name) {
	if (!has_attr(attr_name)) return;
	_attrs.erase(attr_name);
}
void node::remove_attr(const attribute old_attr) {
	remove_attr(old_attr.name);
}

const string& node::operator[](const string &attr_name) const {
	return _attrs.find(attr_name)->second.value;
}
string& node::operator[](const string &attr_name) {
	return _attrs.find(attr_name)->second.value;
}



string node::path() const {
	if (!has_space) return NAMESPACE_DELIMITER + _id;
	else return space()->path + NAMESPACE_DELIMITER + _id;
}