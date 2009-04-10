#ifndef PARTICLE_H
#define PARTICLE_H

struct particle
{
	float x;
	float y;
	float accel;
	float speed;
	float angle;
	float fade;
	float radius;
};

struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle, float fade, float radius);

void particle_destroy(struct particle *p);
void particle_update(struct particle *p);
void particle_draw(const struct particle *p);

int particle_alive(const struct particle *p);

#endif /* PARTICLE_H */
