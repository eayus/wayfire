#ifndef WF_CUBE_CUBEMAP_HPP
#define WF_CUBE_CUBEMAP_HPP

#include "cube-background.hpp"

class wf_cube_background_cubemap : public wf_cube_background_base
{
    public:
    wf_cube_background_cubemap();
    virtual void render_frame(const wf_framebuffer& fb,
        wf_cube_animation_attribs& attribs) override;

    ~wf_cube_background_cubemap();

    private:
    void reload_texture();
    void create_program();

    GLuint program = -1, tex = -1;
    GLuint matrixID, posID;

    std::string last_background_image;
    wf_option background_image;
};

#endif /* end of include guard: WF_CUBE_CUBEMAP_HPP */
