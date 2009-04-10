#ifndef PARTICLE_H
#define PARTICLE_H

struct particle
{
	float x;
	float y;
	float accel;
	float speed;
	float ini_speed;
	float angle;
	float fade;
};

struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle);

void particle_destroy(struct particle *p);
void particle_update(struct particle *p);
void particle_draw(const struct particle *p);
void particle_load_texture(void);

int particle_alive(const struct particle *p);

#endif /* PARTICLE_H */