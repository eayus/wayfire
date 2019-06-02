// WayFire
#include <core.hpp>
#include <output.hpp>
#include <signal-definitions.hpp>
#include "ellis_tiler.hpp"

namespace elos {

	class EllisTiler : public wayfire_plugin_t {
	public:
		void init (wayfire_config* config) override {
			// Initialise superclass properties
			this->grab_interface->name = "ellis_tiler";
			this->grab_interface->abilities_mask = WF_ABILITY_CONTROL_WM;

			// Initialise the window tree
			this->window_tree = WindowTree::empty();

			// Set the callback functions to run on wayfire events
			this->set_callbacks();
		}

		void set_callbacks() {
			auto on_new_view = [=](signal_data* data) {
				wayfire_view view = get_signaled_view(data);
				this->window_tree.insert(view);
			};

			auto on_view_removed = [=](signal_data* data) {
				wayfire_view view = get_signaled_view(data);
				this->window_tree.remove(view);
			};

			this->output->connect_signal("create-view", &on_new_view);
			this->output->connect_signal("attach-view", &on_new_view);
			this->output->connect_signal("detach-view", &on_view_removed);
		}

	private:
		WindowTree window_tree;
	};

}

extern "C" {
	wayfire_plugin_t* newInstance() {
		return new elos::EllisTiler();
	}
}
