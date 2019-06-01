#include "ellis_tiler.hpp"

WindowTree::WindowTree() :
		root(Node((Container){ .split_type = SplitType::Horizontal, .children = std::vector<Node>{} })) {
}

WindowTree WindowTree::empty() {
	WindowTree wt;
	return wt;
}

void WindowTree::insert(wayfire_view view) {
	Window win;
	win.view = view;


	this->root = Node((Container) {
		.split_type = SplitType::Horizontal,
		.children = std::vector<Node>{ &this->root, win },
	});
}

void WindowTree::remove(wayfire_view view) {
	WindowTree::remove_from_node(this->root, view);
}

void WindowTree::remove_from_node(Node* n, wayfire_view view) {
	// TODO: short circuit deletion.

	auto i = n->index();

	if (i == 0) {
		Container con = std::get<0>(*n);

		for (Node child : con.children) {
			WindowTree::remove_from_node(&child, view);
		}
	} else {
		Window win = std::get<1>(*n);

		if (win.view == view) {
			*n = Node((Container) {
				.split_type = SplitType::Horizontal,
				.children = std::vector<Node>{},
			});
		}
	}
}

