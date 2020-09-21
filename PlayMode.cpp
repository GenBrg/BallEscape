#include "PlayMode.hpp"
#include "LitColorTextureProgram.hpp"
#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "Sector.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

Load< MeshBuffer > ball_escape_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer *ret = new MeshBuffer(data_path("ball_escape.pnct"));
	ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > ball_escape_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("ball_escape.scene"),
                  [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = ball_escape_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;
		drawable.mesh = &mesh;
	});
});

PlayMode::PlayMode() : scene(*ball_escape_scene) {
// 	//get pointers to leg for convenience:
// 	for (auto &transform : scene.transforms) {
// 		if (transform.name == "Hip.FL") hip = &transform;
// 		else if (transform.name == "UpperLeg.FL") upper_leg = &transform;
// 		else if (transform.name == "LowerLeg.FL") lower_leg = &transform;
// 	}
// 	if (hip == nullptr) throw std::runtime_error("Hip not found.");
// 	if (upper_leg == nullptr) throw std::runtime_error("Upper leg not found.");
// 	if (lower_leg == nullptr) throw std::runtime_error("Lower leg not found.");

// 	hip_base_rotation = hip->rotation;
// 	upper_leg_base_rotation = upper_leg->rotation;
// 	lower_leg_base_rotation = lower_leg->rotation;


//	auto *transform = new Scene::Transform();
//
//	transform->position[0] = 1;
//	scene.drawables.emplace_back(transform);
//	auto& drawable = scene.drawables.back();
//	drawable.pipeline = lit_color_texture_program_pipeline;
//	drawable.mesh = &(ball_escape_meshes->lookup("Ball"));

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) {
        throw std::runtime_error("Expecting scene to have exactly one camera, but it has "
        + std::to_string(scene.cameras.size()));
	}
	camera = &scene.cameras.front();

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
		// give a force to a ball
		else if (evt.key.keysym.sym == SDLK_UP) {
		    if(!hit_forward.pressed) {
                hit_forward.pressed = true;
                hit_ball = true;
		    }
            return true;
        } else if (evt.key.keysym.sym == SDLK_DOWN) {
            if(!hit_backward.pressed) {
                hit_backward.pressed = true;
                hit_ball = true;
            }
            return true;
        } else if (evt.key.keysym.sym == SDLK_LEFT) {
            if(!hit_left.pressed) {
                hit_left.pressed = true;
                hit_ball = true;
            }
            return true;
        } else if (evt.key.keysym.sym == SDLK_RIGHT) {
            if(!hit_right.pressed) {
                hit_right.pressed = true;
                hit_ball = true;
            }
            return true;
        }
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
        // ball key
        else if (evt.key.keysym.sym == SDLK_UP) {
            hit_forward.pressed = false;
            return true;
        } else if (evt.key.keysym.sym == SDLK_DOWN) {
            hit_backward.pressed = false;
            return true;
        } else if (evt.key.keysym.sym == SDLK_LEFT) {
            hit_left.pressed = false;
            return true;
        } else if (evt.key.keysym.sym == SDLK_RIGHT) {
            hit_right.pressed = false;
            return true;
        }
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			camera->transform->rotation = glm::normalize(
				camera->transform->rotation
				* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
			);
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	// //slowly rotates through [0,1):
	// wobble += elapsed / 10.0f;
	// wobble -= std::floor(wobble);

	// hip->rotation = hip_base_rotation * glm::angleAxis(
	// 	glm::radians(5.0f * std::sin(wobble * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 1.0f, 0.0f)
	// );
	// upper_leg->rotation = upper_leg_base_rotation * glm::angleAxis(
	// 	glm::radians(7.0f * std::sin(wobble * 2.0f * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 0.0f, 1.0f)
	// );
	// lower_leg->rotation = lower_leg_base_rotation * glm::angleAxis(
	// 	glm::radians(10.0f * std::sin(wobble * 3.0f * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 0.0f, 1.0f)
	// );

	//move camera:
	{

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 forward = -frame[2];

		camera->transform->position += move.x * right + move.y * forward;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;

	// update platform
	for(auto &platform: platforms) {
        platform.height += elapsed * platform_up_speed;
	}

    while(platforms.size() >= 1 && platforms.front().height > platform_max_height) {
        //todo while (... or ball already pass this platform)
        platforms.erase(platforms.begin());
    }

    while(platforms.size() < platform_cnt) {
        double init_height = platform_init_height;
        if(platforms.size() >= 1) {
            init_height = platforms.back().height - platform_interval;
        }
        platforms.emplace_back(1 + rand() % 4, rand() % 5, init_height);
    }

    // ball update
    if(hit_ball) {
        // our camera always looks at the central column, the forward hit, always, give
        // a hit forward based on the camera's view
        glm::vec2 init_speed = glm::vec2(0, 0) -
                glm::vec2(camera->transform->position[0], camera->transform->position[1]);
        // normalize
        init_speed = glm::normalize(init_speed);

        if(hit_forward.pressed) {
            ball.hit(init_speed);
            hit_ball = false;
        } else if(hit_backward.pressed) {
            ball.hit(-init_speed);
            hit_ball = false;
        } else if(hit_left.pressed) {
            ball.hit(glm::vec2(-init_speed[1], init_speed[0]));
            hit_ball = false;
        } else if(hit_right.pressed) {
            ball.hit(glm::vec2(init_speed[1], -init_speed[0]));
            hit_ball = false;
        }
    }

    // fade speed of ball
    ball.fade(elapsed * ball_friction_acc); // delta v = a * t
    ball.x += ball.get_speed()[0] * elapsed;
    ball.y += ball.get_speed()[1] * elapsed;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	GL_ERRORS();
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	GL_ERRORS();
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	GL_ERRORS();
	glUseProgram(0);

//    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.


    {
        // draw ball
        auto *transform = new Scene::Transform();
        transform->position[0] = ball.x;
        transform->position[1] = ball.y;
//        transform->position[0] = 1;
        scene.drawables.emplace_back(transform);
        auto& drawable = scene.drawables.back();
        drawable.pipeline = lit_color_texture_program_pipeline;
        drawable.mesh = &(ball_escape_meshes->lookup("Ball"));

    }

    std::vector<Scene::Transform *> temp_sec_transforms;
    size_t before_size = scene.drawables.size();
    {
        // push all platforms' sectors to drawables vector
        for (auto &platform: platforms) {
            for (size_t i = 0; i < platform.sectors.size(); i++) {
                // Add sectors
                auto transform_p = platform.get_transform(i);
                scene.drawables.emplace_back(transform_p);
                auto &drawable = scene.drawables.back();
                drawable.pipeline = lit_color_texture_program_pipeline;
                drawable.mesh = &(ball_escape_meshes->lookup(platform.sectors[i].get_mesh_name()));
                temp_sec_transforms.push_back(transform_p);
            }
        }
    }

    scene.draw(*camera);

    {
        // pop all platforms' sectors out of drawables vector
        int delete_cnt = (int) (scene.drawables.size() - before_size);
        for(int i=0; i<delete_cnt; i++) {
            scene.drawables.pop_back();
        }
        for(auto t_p: temp_sec_transforms) {
            delete t_p;
        }
    }

    {
        //clear ball
        scene.drawables.pop_back();
    }


	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
}
