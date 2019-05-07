#pragma once
#ifndef __RND_GEN_CORE_GEN_TREE_H__
#define __RND_GEN_CORE_GEN_TREE_H__

#include "xml-wrapper.h"
#include "rapidxml.hpp"

namespace xml {
namespace rxml = rapidxml;

using xml_node = rxml::xml_node<>;
using xml_document = rxml::xml_document<>;
using xml_attribute = rxml::xml_attribute<>;

class node;
class node_ref;
class document;
class name_space;
class xml_wrapper;

typedef node* (*node_factory)(xml_wrapper* wrapper, node* parent, const xml_node* node);

template<class node_type>
typedef node_type* factory_for_(xml_wrapper* wrapper, node* parent, const xml_node* base_node) {
	return new node_type(doc, base_node);
}

enum xml_errno {
	NO_ERROR,
	NO_ID,
	NO_REFERENCE,
	REF_WITH_ID,
	REF_TO_REF,
	REF_ID_MISMATCH,
	CHILD_NOT_FOUND,
	NODE_NOT_FOUND,
	ATTRIBUTE_NOT_FOUND,
	NAMESPACE_COLLISION,
	FLOATING_REFERENCE,
	REFERENCE_REBIND,

	OTHER_ERROR
};

const char* ID_STRING = "id";
const unsigned int MIN_ID_LENGTH = 1;
const char* REF_STRING = "ref";
const char* DEFAULT_VALUE = "";
const char* NAMESPACE_DELIMITER = "::";

class node {
protected:
	string _id;
	name_space* _space;
	node* _parent;
	hashmap<string, node*> _children;
	hashmap<string, attribute> _attrs;
public:
	static const string classname;

	node();
	node(xml_wrapper* wrapper, node* parent, const xml_node* node);
	node(const string& id);
	node(node* parent, const string& id);
	node(node* parent, const string& id, const vector<attribute> &attributes);
	~node();

	virtual bool instanceof(const string &classname) const;

	virtual string id() const;

	virtual vector<const node*> children() const;
	virtual vector<node*> children();
	virtual const node* child(const string &name) const;
	virtual node* child(const string &name);
	virtual bool has_child(const node* child) const;
	virtual bool has_child(const string &child_name) const;
	virtual void add_child(node* new_child);
	virtual void remove_child(node* old_child);

	bool has_parent() const;
	const node* parent() const;
	node* parent();
	void parent(node* new_parent);

	bool has_space() const;
	const name_space* space() const;
	name_space* space();
	virtual void space(name_space* new_space);
	virtual const name_space* child_space() const;
	virtual name_space* child_space();

	virtual vector<attribute> attributes() const;
	virtual string attr(const string &attr_name) const;
	virtual void attr(const string &attr_name, const string &attr_value);
	virtual void attr(const attribute attr_pair);
	virtual bool has_attr(const attribute attr) const;
	virtual bool has_attr(const string &attr_name) const;
	virtual void add_attr(const attribute attr);
	virtual void add_attr(const string &attr_name);
	virtual void add_attr(const string &attr_name, const string& attr_value);
	virtual void remove_attr(const attribute attr);
	virtual void remove_attr(const string &attr_name);
	virtual const string& operator[](const string &attr_name) const;
	virtual string& operator[](const string &attr_name);

	string path() const;
};

class node_ref : public node {
protected:
	string _base_id;
	node* _base;
public:
	static const string classname;

	node_ref();
	node_ref(xml_wrapper* wrapper, node* parent, const xml_node *node);
	node_ref(node* parent, const string &ref_id);
	~node_ref();

	virtual bool instanceof(const string &classname) const;

	bool bound() const;

	const node* base() const;
	node* base();
	void base(node* new_base);

	virtual string id() const;

	virtual vector<const node*> children() const;
	virtual vector<node*> children();
	virtual const node* child(const string &name) const;
	virtual node* child(const string &name);
	virtual bool has_child(const node* child) const;
	virtual bool has_child(const string &child_name) const;
	virtual void add_child(node* new_child);
	virtual void remove_child(node* old_child);

	virtual void space(name_space* new_space);

	virtual vector<attribute> attributes() const;
	virtual string attr(const string &attr_name) const;
	virtual void attr(const string &attr_name, const string &attr_value);
	virtual const string& operator[](const string &attr_name) const;
	virtual string& operator[](const string &attr_name);
	virtual bool has_attr(const attribute attr) const;
	virtual bool has_attr(const string &attr_name) const;
	virtual void add_attr(const attribute &attr);
	virtual void add_attr(const string &attr_name);
	virtual void add_attr(const string &attr_name, const string& attr_value);
	virtual void remove_attr(const attribute attribute);
	virtual void remove_attr(const string &attr_name);
};

class name_space : public node {
private:
	hashmap<string, node*> _nodes;				// List of all non-reference nodes in a space
	hashmap<string, vector<node_ref*>> _refs;	// Map a node name to its references
public:
	static const string classname;

	name_space();
	name_space(xml_wrapper* wrapper, node* parent, const xml_node* base_node);
	~name_space();

	virtual const name_space* child_space() const;
	virtual name_space* child_space();

	virtual vector<const node*> nodes() const;
	virtual vector<node*> nodes();
	virtual const node* get(const string &node_name) const;
	virtual node* get(const string &node_name);
	virtual vector<const node_ref*> refs(const string &node_name) const;
	virtual vector<node_ref*> refs(const string &node_name);
	virtual void add_node(node* new_node);
	virtual void remove_node(node* old_node);
	virtual void add_ref(node_ref* new_node);
	virtual void remove_ref(node_ref* old_node);
};

class document : public name_space {
public:
	static const string classname;

	document();
	document(xml_wrapper* wrapper, xml_node* base_node);
	~document();
};

class xml_wrapper {
private:
	hashmap<string, document*> _docs;
	hashmap<string, node_factory> _factories;
public:
	xml_wrapper();
	xml_wrapper(const string &xml_file);
	~xml_wrapper();

	template<class node_type>
	void register_() {
		_factories[node_type::classname] = factory_for_<node_type>;
	}

	vector<const document*> docs() const;
	vector<document*> docs();
	const document* doc(const string &doc_name) const;
	document* doc(const string &doc_name);
	bool has_doc(const string &doc_name) const;
	void add_doc(const string& xml_file);
	void remove_doc(const string& doc_name);
	virtual const document& operator[](const string &doc_name) const;
	virtual document& operator[](const string &doc_name);

	node* create_node(node* parent, const xml_node* base_node);
};
}

#endif // !__RND_GEN_CORE_GEN_TREE_H__