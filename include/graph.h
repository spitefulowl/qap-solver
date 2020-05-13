#pragma once
#include <cstddef>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdexcept>

template<typename T>
class node {
private:
	using value_type = T;
	using pointer = T*;
	using node_type = node<T>;
	using node_type_ptr = node_type*;
public:
	// constructors
	node();
	template<typename U>
	node(U&& value, std::size_t level);

	// info functions
	T& get_value();

	// public node operations

	node_type_ptr get_by_index(std::size_t idx) {
		if (idx < my_next_nodes.size()) {

			return my_next_nodes[idx];
		}
		else {
			throw std::runtime_error("Out of index");
		}
	}
	std::size_t get_level();

	bool valid_node_value() const {
		if (value) {
			return true;
		}
		else {
			return false;
		}
	}

	std::size_t count_of_nodes() const {
		return my_next_nodes.size();
	}

private:
	// private info modifiers functions
	void set_level(std::size_t value) {
		this->my_level = value;
	}

	void internal_insert(node_type_ptr value) {
		my_next_nodes.push_back(value);
	}

	bool internal_destroy(node_type_ptr value);

public:
	// destructor helper
	void destroy_all();

	// destructor
	~node();
private:
	pointer value;
	std::size_t my_level;
	node_type_ptr back_node;
	std::vector<node_type_ptr> my_next_nodes;
	template<typename K> friend class graph;
};

template<typename T>
class graph {
private:
	using node_type = node<T>;
	using node_type_ptr = node_type*;
public:
	// constructors
	graph();

	// node operations

	template<typename U>
	node_type_ptr make_node(U&& value); // TODO: check that U and T is same
	bool delete_node(node_type_ptr place, node_type_ptr target);
	void insert_node(node_type_ptr place, node_type_ptr target);
	void reset() {
		current_node = base_node;
	}

	node_type_ptr get_base_node() {
		return base_node;
	}

	void set_next_nodes(const std::vector<node_type_ptr>& set);
	void select_next(std::size_t idx);
	bool try_select_next(std::size_t idx);
	node_type_ptr get_current_node() {
		return current_node;
	}
	std::size_t count_of_nodes() const;
	std::size_t base_count_of_nodes() const;
	void set_current_node(node_type_ptr node) {
		current_node = node;
	}

	bool back();

	void destroy_by_index(std::size_t idx) {
		if (current_node->my_next_nodes[idx]) {
			delete current_node->my_next_nodes[idx];
			current_node->my_next_nodes[idx] = nullptr;
		}
	}

	// destructor
	~graph();
private:

	bool valid_node(std::size_t idx) const {
		if (current_node->my_next_nodes[idx]) {
			return true;
		}
		else {
			return false;
		}
	}

	node_type_ptr base_node;
	node_type_ptr current_node;
};


// node

// constructors
template<typename T>
node<T>::node() : my_level(static_cast<std::size_t>(0)), value(nullptr), back_node(nullptr) {}

template<typename T> 
template<typename U>
node<T>::node(U&& value, std::size_t level) : my_level(level) { // �������� ���� U � T (����� ������������ ������������� ������ ���� ����������)
	this->value = new T(std::forward<U>(value));

}

// destructors
template<typename T>
node<T>::~node() {
	if (value) {
		delete value;
		value = nullptr;
	}
	destroy_all();
}

// info functions
template<typename T>
std::size_t node<T>::get_level() {
	return my_level;
}

template<typename T>
T& node<T>::get_value() { // maybe return reference to value ?
	if (value) {
		return *value;
	}
	else {
		throw std::runtime_error("Zero pointer");
	}
}

// internal node operations

template<typename T>
bool node<T>::internal_destroy(node_type_ptr value) {
	auto iterator = std::find(my_next_nodes.begin(), my_next_nodes.end(), value);
	if (iterator != my_next_nodes.end()) {
		my_next_nodes.erase(iterator);
		return true;
	}
	else {
		return false;
	}
}

// destructor helper
template<typename T>
void node<T>::destroy_all() {
	for (auto& elem : my_next_nodes) {
		if (elem) {
			delete elem;
			elem = nullptr;
		}
	}
	if (value) {
		delete value;
		value = nullptr;
	}
}



// graph

template<typename T> 
graph<T>::graph() {
	base_node = new node_type;
	current_node = base_node;
}
template<typename T> 
graph<T>::~graph() {
	if (base_node) {
		delete base_node;
	}
}

template<typename T> 
template<typename U>
typename graph<T>::node_type_ptr graph<T>::make_node(U&& value) {
	node_type_ptr my_node = new node_type(std::forward<T>(value), static_cast<std::size_t>(0));
	return my_node;
}

template<typename T> 
void graph<T>::insert_node(node_type_ptr place, node_type_ptr target) {
	target->set_level(place->get_level() + 1);
	place->internal_insert(target);
}

template<typename T>
bool graph<T>::delete_node(node_type_ptr place, node_type_ptr target) {
	return place->internal_destroy(target);
}

template<typename T>
std::size_t graph<T>::count_of_nodes() const {
	return current_node->count_of_nodes();
}

template<typename T>
std::size_t graph<T>::base_count_of_nodes() const {
	return base_node->count_of_nodes();
}

template<typename T>
bool graph<T>::back() {
	if (current_node->back_node) {
		current_node = current_node->back_node;
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
void graph<T>::set_next_nodes(const std::vector<node_type_ptr>& set)
{
	if (current_node->count_of_nodes() == 0) {
		current_node->my_next_nodes = set; // TODO: check correct
		std::size_t new_level = current_node->get_level() + 1;
		for (auto& elem : current_node->my_next_nodes) {
			elem->back_node = current_node;
			elem->set_level(new_level);
		}
	}
	else {
		throw std::runtime_error("my_next_nodes is not empty");
	}
}

template<typename T>
void graph<T>::select_next(std::size_t idx)
{
	if (idx < count_of_nodes()) {
		current_node = current_node->my_next_nodes[idx];
	}
	else {
		throw std::runtime_error("Invalid index");
	}
}

template<typename T>
bool graph<T>::try_select_next(std::size_t idx)
{
	if (idx < count_of_nodes()) {
		if (valid_node(idx)) {
			current_node = current_node->my_next_nodes[idx];
			return true;
		}
		else {
			return false;
		}
	}
	else {
		throw std::runtime_error("Invalid index");
	}
}
