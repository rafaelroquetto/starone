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
	float r;
	float g;
	float b;
	float alpha;
};

struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle);

void particle_destroy(struct particle *p);
void particle_update(struct particle *p);
void particle_draw(const struct particle *p);
void particle_set_color(struct particle *p,
		float r, float g, float b);

int particle_alive(const struct particle *p);

#endif /* PARTICLE_H */
