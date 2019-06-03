// WayFire
#include <core.hpp>
#include <output.hpp>
#include <signal-definitions.hpp>
#include "ellis_tiler.hpp"
#include <iostream>
#include <libevdev-1.0/libevdev/libevdev.h>

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

			// Setup keyboard shortcuts
			this->initialise_keyboard_shortcuts();
		}

		void register_callbacks() {
			std::cout << "registering callbacks" << std::endl;
			this->output->connect_signal("create-view", &this->on_new_view);
			this->output->connect_signal("attach-view", &this->on_new_view);
			this->output->connect_signal("detach-view", &this->on_remove_view);
		}

		void initialise_callbacks() {
			std::cout << "initialising callbacks" << std::endl;
			this->on_new_view = [=](signal_data* data) {
				wayfire_view view = get_signaled_view(data);
				this->window_tree.insert(view);
			};

			this->on_remove_view = [=](signal_data* data) {
				wayfire_view view = get_signaled_view(data);
				this->window_tree.remove(view);
			};
		}

		void initialise_keyboard_shortcuts() {
			this->on_split_hori = [=](uint32_t) {
				this->window_tree.next_split(SplitType::Horizontal);
			};

			this->on_split_vert = [=](uint32_t) {
				this->window_tree.next_split(SplitType::Vertical);
			};

			this->output->add_key(new_static_option("<super> KEY_O"), &this->on_split_hori);
			this->output->add_key(new_static_option("<super> KEY_P"), &this->on_split_vert);
		}

	private:
		elos::WindowTree window_tree;

		signal_callback_t on_new_view;
		signal_callback_t on_remove_view;

		key_callback on_split_hori;
		key_callback on_split_vert;
	};

}

extern "C" {
	wayfire_plugin_t* newInstance() {
		return new elos::EllisTiler();
	}
}
