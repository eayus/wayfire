#ifndef WF_CUBE_BACKGROUND_SKYDOME
#define WF_CUBE_BACKGROUND_SKYDOME

#include "cube-background.hpp"
#include <vector>

class wf_cube_background_skydome : public wf_cube_background_base
{
    public:
    wf_cube_background_skydome(wayfire_output *output);
    virtual void render_frame(const wf_framebuffer& fb,
        wf_cube_animation_attribs& attribs) override;

    virtual ~wf_cube_background_skydome();

    private:
    wayfire_output *output;

    void load_program();
    void fill_vertices();
    void reload_texture();

    GLuint program = -1, tex = -1;
    GLuint posID, uvID, modelID, vpID;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> coords;
    std::vector<GLuint> indices;

    std::string last_background_image;
    int last_mirror = -1;

    wf_option background_image, mirror_opt;
};

#endif /* end of include guard: WF_CUBE_BACKGROUND_SKYDOME */
