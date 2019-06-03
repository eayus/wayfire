#include "ellis_tiler.hpp"

#include <iostream>
#include <algorithm>

constexpr int OUTER_GAP = 20;
constexpr int INNER_GAP = 20;

namespace elos {

	WindowTree::WindowTree() :
			root(nullptr), next_split_type(SplitType::Horizontal) {
	}

	WindowTree WindowTree::empty() {
		WindowTree wt;
		return wt;
	}

	void WindowTree::next_split(SplitType type) {
		this->next_split_type = type;
	}

	void WindowTree::insert(wayfire_view view) {
		std::cout << "Inserting window into tree" << std::endl;

		if (this->root != nullptr) {
			this->root = new Node((Container){
				.split_type = this->next_split_type,
				.left = this->root,
				.right = new Node((Window){ .view = view }),
			});
		} else {
			this->root = new Node((Window){ .view = view });
		}

		// TODO: Better geometry system
		this->update_all_geometry();
	}

	void WindowTree::remove(wayfire_view view) {
		if (this->root == nullptr) return;

		auto node_type = this->root->which();

		if (node_type == 0) {
			WindowTree::remove_from_container(&this->root, view);
		} else {
			Window& win = boost::get<Window>(*this->root);

			if (win.view == view) {
				delete this->root;
				this->root = nullptr;
			}
		}

		// TODO: Better geometry system
		this->update_all_geometry();
	}

	void WindowTree::remove_from_container(Node** n, wayfire_view view) {
		Container& con = boost::get<Container>(**n);

		if (con.left->which() == 1 && boost::get<Window>(*con.left).view == view) {
			*n = con.right;
			delete con.left;
			return;
		}

		if (con.right->which() == 1 && boost::get<Window>(*con.right).view == view) {
			*n = con.left;
			delete con.right;
			return;
		}

		if (con.left->which() == 0) {
			WindowTree::remove_from_container(&con.left, view);
		}

		if (con.right->which() == 0) {
			WindowTree::remove_from_container(&con.right, view);
		}
	}

	void WindowTree::update_all_geometry() {
		// TODO: don't hardcode
		std::cout << "Updating all geomtry" << std::endl;

		wf_geometry new_dims = (wf_geometry){
			.x = 0 + OUTER_GAP,
			.y = 0 + OUTER_GAP,
			.width = 1920 - (2 * OUTER_GAP),
			.height = 1080 - (2 * OUTER_GAP),
		};

		if (this->root == nullptr) return;


		WindowTree::update_geometry(this->root, new_dims);
	}

	void WindowTree::update_geometry(Node* n, wf_geometry new_dims) {
		auto i = n->which();

		std::cout << i << std::endl;
		if (i == 0) {
			Container& con = boost::get<Container>(*n);

			wf_geometry left_dims = new_dims;
			wf_geometry right_dims = new_dims;

			auto half_w = new_dims.width / 2;
			auto half_h = new_dims.height / 2;

			auto half_g = INNER_GAP / 2;

			if (con.split_type == SplitType::Horizontal) {
				left_dims.width = half_w - half_g;
				right_dims.width = half_w - half_g;
				right_dims.x += half_w + half_g;
			} else {
				left_dims.height = half_h - half_g;
				right_dims.height = half_h - half_g;
				right_dims.y += half_h + half_g;
			}

			WindowTree::update_geometry(con.left, left_dims);
			WindowTree::update_geometry(con.right, right_dims);
		} else {
			Window& win = boost::get<Window>(*n);

			win.view->set_geometry(new_dims);
		}
	}
}
