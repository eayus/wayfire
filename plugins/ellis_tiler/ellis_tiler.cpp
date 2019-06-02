#include "ellis_tiler.hpp"

namespace elos {

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
			.children = std::vector<Node>{ this->root, Node(win) },
		});

		// TODO: Better geometry system
		this->update_all_geometry();
	}

	void WindowTree::remove(wayfire_view view) {
		WindowTree::remove_from_node(this->root, view);

		// TODO: Better geometry system
		this->update_all_geometry();
	}

	void WindowTree::remove_from_node(Node* n, wayfire_view view) {
		// TODO: short circuit deletion.

		auto i = n->index();

		if (i == 0) {
			Container con = boost::get<Container>(*n);

			for (Node child : con.children) {
				WindowTree::remove_from_node(&child, view);
			}
		} else {
			Window win = boost::get<Window>(*n);

			if (win.view == view) {
				*n = Node((Container) {
					.split_type = SplitType::Horizontal,
					.children = std::vector<Node>{},
				});
			}
		}
	}

	void WindowTree::update_all_geometry() {
		// TODO: don't hardcode

		wf_geometry dims = (wf_geometry){
			.x = 0,
			.y = 0,
			.width = 1920,
			.height = 1080,
		};

		WindowTree::update_geometry(this->root, dims);
	}

	void WindowTree::update_geometry(Node& n, wf_geometry new_dims) {
		auto i = n->index();

		if (i == 0) {
			Container con = boost::get<Container>(n);
			auto children = con.children.size();

			if (con.split_type == SplitType::Horizontal) {
				auto child_width = new_dims.width / children;

				for (int j = 0; j < children, j++) {
					auto child_dims = (wf_geometry){
						.x = new_dims.x + (j * child_width),
						.y = new_dims.y
						.width = child_width,
						.height = new_dims.height,
					};

					WindowTree::update_geometry(con.children[j], child_dims);
				}
			} else {
				auto child_height = new_dims.height / children;

				for (int j = 0; j < children, j++) {
					auto child_dims = (wf_geometry){
						.x = new_dims.x
						.y = new_dims.y + (j * child_height),
						.width = new_dims.width,
						.height = child_height,
					};

					WindowTree::update_geometry(con.children[j], child_dims);
				}
			}
		} else {
			Window win = boost::get<Window>(n);

			win.view->set_geometry(new_dims);
		}
	}
}
