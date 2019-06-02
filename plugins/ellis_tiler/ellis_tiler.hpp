#pragma once

// Standard Library
#include <vector>
#include <boost/variant.hpp>

// WayFire
#include <view.hpp>
#include <output.hpp>

namespace elos {

	struct Container;
	struct Window;


	using Node = boost::variant<Container, Window>;

	enum class SplitType {
		Horizontal,
		Vertical,
	};

	struct Container {
		SplitType split_type;
		std::vector<Node> children;
	};

	struct Window {
		wayfire_view view;
	};

	class WindowTree {
	public:
		static WindowTree empty();

		void insert(wayfire_view view);
		void remove(wayfire_view view);




		WindowTree();

	private:
		void update_all_geometry();

		static void remove_from_node(Node* n, wayfire_view view);
		static void update_geometry(Node& n, wf_geometry new_dims);

		Node root;
	};

}
