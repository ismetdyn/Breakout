#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <texture.h>
#include <game_object.h>
#include <power_up.h>

/* BallObject holds the state of the Ball object inheriting
 * relevant state data from GameObject.
 */
class BallObject : public GameObject
{
public:
	// Ball state
	GLfloat		Radius;
	GLboolean	Stuck;
	GLboolean	Sticky, PassThrough;
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, 
		Texture2D sprite);
	glm::vec2	Move(GLfloat dt, GLuint window_width);
	void		Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif // !BALLOBJECT_H