#include "Mode.hpp"
#include "Scene.hpp"
#include "Platform.hpp"
#include "Ball.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	struct HitBallButton {
	    bool pressed = false;
	} hit_forward, hit_backward, hit_left, hit_right;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	//hexapod leg to wobble:
//	Scene::Transform *hip = nullptr;
//	Scene::Transform *upper_leg = nullptr;
//	Scene::Transform *lower_leg = nullptr;
//	glm::quat hip_base_rotation;
//	glm::quat upper_leg_base_rotation;
//	glm::quat lower_leg_base_rotation;
//	float wobble = 0.0f;
	
	//camera:
	Scene::Camera *camera = nullptr;

	// all platforms
	std::deque<Platform> platforms;

	const double deconstruct_last_sec = 5.0f;
	std::deque<Platform> deconstructing_platforms;

	// moving speed up
    double platform_up_speed = 0.6;
	double platform_interval = 2.5;
	double platform_init_height = -4.0f;

	// make sure there are at least 10 platforms in the vector
	size_t platform_cnt = 5;

	const double platform_max_height = 3.0f;

	//
	Ball ball;
	float ball_init_vel_per_hit = 0.6f; // init speed

    bool hit_ball = false;


    //
    int player_life = 5;
    int total_score = 0;
};
