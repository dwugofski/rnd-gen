
#include "core.h"

using namespace xml;

const string node_ref::classname = "node_ref";

node_ref::node_ref() : node() {
	_base = 0;
}

node_ref::node_ref(xml_wrapper* wrapper, node* my_parent, const xml_node* base_node) : node_ref() {
	const xml_attribute* curr_attr = base_node->first_attribute();
	while (curr_attr != 0) {
		add_attr(std::tolower(curr_attr->name(), std::locale()), curr_attr->value());
		curr_attr = curr_attr->next_attribute();
	}
	if (has_attr(ID_STRING)) throw(REF_WITH_ID);
	if (!has_attr(REF_STRING)) throw(NO_REFERENCE);
	else if (attr(REF_STRING).size() < MIN_ID_LENGTH) throw(NO_ID);
	else _base_id = attr(REF_STRING);

	_parent = my_parent;
	if (_parent != 0) my_parent->add_child(this);
}

node_ref::node_ref(node* my_parent, const string &ref_id) {
	if (ref_id.size() < MIN_ID_LENGTH) throw(NO_ID);
	else _base_id = ref_id;
}

node_ref::~node_ref() { }

bool node_ref::instanceof(const string& class_name) const {
	if (class_name == node::classname) return true;
	if (class_name == node_ref::classname) return true;
	return false;
}

bool node_ref::bound() const {
	return _base != 0;
}

const node* node_ref::base() const {
	return static_cast<const node*>(_base);
}
node* node_ref::base() {
	return const_cast<node*>(static_cast<const node_ref*>(this)->base());
}

void node_ref::base(node* new_base) {
	if (new_base != 0 && new_base != _base && bound()) throw(REFERENCE_REBIND);
	_base = new_base;
	_space->add_node(new_base); // Unfortunately, must bind within namepsace
}

vector<const node*> node_ref::children() const {
	return static_cast<const node*>(_base)->children();
}
vector<node*> node_ref::children() {
	return _base->children();
}


