#pragma once

// Standard Library
#include <vector>
#include <utility>
#include <boost/variant.hpp>
#include <boost/optional.hpp>

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

		Node* left; // not null
		Node* right; // not null
	};

	struct Window {
		wayfire_view view;
	};

	class WindowTree {
	public:
		static WindowTree empty();

		void insert(wayfire_view view);
		void remove(wayfire_view view);

		void next_split(SplitType type);



		WindowTree();

	private:
		void update_all_geometry();

		static void remove_from_container(Node** n, wayfire_view view);
		static void update_geometry(Node* n, wf_geometry new_dims);

		Node* root; // nullable
		SplitType next_split_type;
	};

}
