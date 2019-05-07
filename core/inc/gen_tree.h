#ifndef __RND_GEN_CORE_GEN_TREE_H__
#define __RND_GEN_CORE_GEN_TREE_H__

#include "core.h"

using namespace rnd_gen;

class Generator;
class Item;
class ItemRef;
class Group;
class GroupRef;
class Option;
class OptionRef;
class GroupOption;
class GroupOptionRef;

typedef Item* (*item_factory)(Generator &gen, XMLNode &node);

class Item {
protected:
	unsigned int _id;
	string _name;
	Generator &_generator;
	Item* _parent;
	hashmap<string, Item*> _children;
	hashmap<string, attribute> _attrs;
public:
	static const string classname;

	Item();
	Item(Generator& gen, const XMLNode &node);
	~Item();

	static bool is(const Item* item);
	virtual bool instanceof(const string &classname) const;

	unsigned int id() const;
	void id(unsigned int new_id);

	string name() const;
	void name(const string& new_name);

	const Generator* generator() const;
	Generator* generator();

	virtual vector<const Item*> children() const;
	virtual vector<Item*> children();
	virtual const Item* child(const string &name) const;
	virtual Item* child(const string &name);
	virtual const Item* child(unsigned int id) const;
	virtual Item* child(unsigned int id);
	virtual const Item* child(const vector<unsigned int> id) const;
	virtual Item* child(const vector<unsigned int> id);
	virtual bool has_child(const Item* child) const;
	virtual bool has_child(const string &child_name) const;
	virtual bool has_child(unsigned int child_id) const;
	virtual bool has_child(const vector<unsigned int> &child_path) const;
	virtual void add_child(Item* new_child);
	virtual void add_child(const XMLNode &child_node);
	virtual void remove_child(Item* old_child);
	virtual void remove_child(const string &child_name);
	virtual void remove_child(unsigned int child_id);
	virtual void remove_child(const vector<unsigned int> &child_path);

	bool has_parent() const;
	const Item* parent() const;
	Item* parent();
	void parent(Item* new_parent);

	virtual vector<const attribute> attributes() const;
	virtual vector<attribute> attributes();
	virtual string attr(const string &attr_name) const;
	virtual void attr(const string &attr_name, const string &attr_value);
	virtual bool has_attr(const attribute attr) const;
	virtual bool has_attr(const string &attr_name) const;
	virtual void add_attr(const attribute &attr);
	virtual void add_attr(const string &attr_name);
	virtual void add_attr(const string &attr_name, const string& attr_value);
	virtual void remove_attr(const attribute attribute);
	virtual void remove_attr(const string &attr_name);
	virtual const string& operator[](const string &attr_name) const;
	virtual string& operator[](const string &attr_name);

	virtual string evaluate() const;
};

class ItemRef : public Item {
protected:
	Item* _base;
public:
	static const string classname;

	ItemRef();
	ItemRef(Generator& gen, const XMLNode &node);
	ItemRef(Item* base);
	~ItemRef();

	virtual bool instanceof(const string &classname) const;

	const Item* base() const;
	Item* base();
	void base(Item* new_base);

	virtual vector<const Item*> children() const;
	virtual vector<Item*> children();
	virtual const Item* child(const string &name) const;
	virtual Item* child(const string &name);
	virtual const Item* child(unsigned int id) const;
	virtual Item* child(unsigned int id);
	virtual const Item* child(const vector<unsigned int> id) const;
	virtual Item* child(const vector<unsigned int> id);
	virtual bool has_child(const Item* child) const;
	virtual bool has_child(const string &child_name) const;
	virtual bool has_child(unsigned int child_id) const;
	virtual bool has_child(const vector<unsigned int> &child_path) const;
	virtual void add_child(Item* new_child);
	virtual void add_child(const XMLNode &child_node);
	virtual void remove_child(Item* old_child);
	virtual void remove_child(const string &child_name);
	virtual void remove_child(unsigned int child_id);
	virtual void remove_child(const vector<unsigned int> &child_path);

	virtual vector<const attribute> attributes() const;
	virtual vector<attribute> attributes();
	virtual string attr(const string &attr_name) const;
	virtual void attr(const string &attr_name, const string &attr_value);
	virtual bool has_attr(const attribute attr) const;
	virtual bool has_attr(const string &attr_name) const;
	virtual void add_attr(const attribute &attr);
	virtual void add_attr(const string &attr_name);
	virtual void add_attr(const string &attr_name, const string& attr_value);
	virtual void remove_attr(const attribute attribute);
	virtual void remove_attr(const string &attr_name);

	virtual string evaluate() const;
};

class Generator : public Item {
private:
	hashmap<unsigned int, Item*> _items;
	hashmap<string, item_factory> _factories;
	string _space;
public:
	Generator();
	~Generator();

	virtual string evaluate() const;
};

class Group : public Item {
public:
	static const string classname;

	Group();
	Group(Generator& gen, const XMLNode &node);
	~Group();

	virtual bool instanceof(const string &classname) const;

	virtual string evaluate() const;
};

class GroupRef : public Group, public ItemRef {
public:
	static const string classname;

	GroupRef();
	GroupRef(Generator &gen, const XMLNode &node);
	GroupRef(Group* base);
	~GroupRef();

	virtual bool instanceof(const string &classname) const;

	virtual string evaluate() const;
};

class Option : public Item {
protected:
	unsigned int _weight;
public:
	static const string classname;
	static const unsigned int default_weight;

	Option();
	Option(Generator& gen, const XMLNode &node);
	~Option();

	virtual bool instanceof(const string &classname) const;

	virtual unsigned int weight() const;
	virtual void weight(unsigned int new_weight);

	virtual string evaluate() const;
};

class OptionRef : public Option, public ItemRef {
public:
	static const string classname;

	OptionRef();
	OptionRef(Generator &gen, const XMLNode &node);
	OptionRef(Option* base);
	~OptionRef();

	virtual bool instanceof(const string &classname) const;

	virtual unsigned int weight() const;
	virtual void weight(unsigned int new_weight);

	virtual string evaluate() const;
};

class GroupOption : public Option, public Group {
public:
	static const string classname;
	static const unsigned int default_weight;

	GroupOption();
	GroupOption(Generator& gen, const XMLNode &node);
	~GroupOption();

	virtual bool instanceof(const string &classname) const;

	virtual string evaluate() const;
};

class GroupOptionRef : public GroupOption, public GroupRef {
public:
	static const string classname;
	static const unsigned int default_weight;

	GroupOptionRef();
	GroupOptionRef(Generator& gen, const XMLNode &node);
	~GroupOptionRef();

	virtual bool instanceof(const string &classname) const;

	virtual string evaluate() const;
};

#endif // !__RND_GEN_CORE_GEN_TREE_H__