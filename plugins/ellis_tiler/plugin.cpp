// WayFire
#include <core.hpp>
#include <output.hpp>
#include <signal-definitions.hpp>
#include "ellis_tiler.hpp"

namespace elos {

	class EllisTiler : public wayfire_plugin_t {
	public:
		void init(wayfire_config* config) override {
			// Initialise superclass properties
			this->grab_interface->name = "ellis_tiler";
			this->grab_interface->abilities_mask = WF_ABILITY_CONTROL_WM;

			// Initialise the window tree
			this->window_tree = WindowTree::empty();

			// Set the callback functions to run on wayfire events
			this->register_callbacks();
			this->initialise_callbacks();
		}

		void register_callbacks() {
			this->output->connect_signal("create-view", &this->on_new_view);
			this->output->connect_signal("attach-view", &this->on_new_view);
			this->output->connect_signal("detach-view", &this->on_remove_view);
		}

		void initialise_callbacks() {
			this->on_new_view = [=](signal_data* data) {
				wayfire_view view = get_signaled_view(data);
				this->window_tree.insert(view);
			};

			this->on_view_removed = [=](signal_data* data) {
				wayfire_view view = get_signaled_view(data);
				this->window_tree.remove(view);
			};
		}

	private:
		WindowTree window_tree;

		signal_callback_t on_new_view;
		signal_callback_t on_remove_view;
	};

}

extern "C" {
	wayfire_plugin_t* newInstance() {
		return new elos::EllisTiler();
	}
}
