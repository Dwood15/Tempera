#include <math.h>
#include <string.h>
#include "../halo_data/table.h"
#include "../hooks/tick.h"
#include "../../math/data_types.h"
#include "particle.h"

struct InterParticle {
    bool valid;
    Vector3D position;
};

struct Particle {
    // 0x0
    uint32_t unknown0;
    uint32_t tag_id_maybe;
    uint32_t unknown1 = 0xFFFFFFFF;
    uint32_t unknown2;
    // 0x10
    uint32_t frames;
    float a0; // 0.0 = begin to fade out
    float a;
    float b;
    // 0x20
    float c; // idk, don't set this to anything less than or equal to 0 or the game will freeze with max CPU usage somehow
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t some_id;
    // 0x30
    Vector3D position;
    float unknownx0;
    // 0x40
    float unknowny0;
    float unknownz0;
    float unknownx1;
    float unknowny1;
    // 0x50
    float unknownz1;
    float radius_x;
    float radius_y;
    float radius_z;
    // 0x60
    uint32_t unknown5;
    float red;
    float green;
    float blue;
};

static InterParticle particles_buffer_0[1024] = {};
static InterParticle particles_buffer_1[1024] = {};

// This is used to verify that a particle hasn't been changed.
static InterParticle particles_buffer_intermediate[1024] = {};

static int32_t particles_count = 0;

void do_particle_interpolation() noexcept {
    static int32_t tick_before = 0;
    auto tick_now = tick_count();

    auto &particle_table = get_particle_table();
    if(tick_before != tick_now) {
        tick_before = tick_now;
        particles_count = particle_table.size;
        if(particles_count > 1024) particles_count = 1024;
        auto *live_particles = reinterpret_cast<Particle *>(particle_table.first);
        memset(particles_buffer_0,0,sizeof(particles_buffer_0));
        for(uint32_t i=0;i<1024;i++) {
            particles_buffer_0[i].position = live_particles[i].position;
            if((live_particles[i].unknown0 & 0xFFFF) != 0) {
                if(distance_squared(particles_buffer_0[i].position, particles_buffer_1[i].position) > 0.45) continue;
                if(live_particles[i].position.x != particles_buffer_intermediate[i].position.x ||
                live_particles[i].position.y != particles_buffer_intermediate[i].position.y ||
                live_particles[i].position.z != particles_buffer_intermediate[i].position.z) continue;
                particles_buffer_0[i].valid = true;
            }
        }
    }
    auto *live_particles = reinterpret_cast<Particle *>(particle_table.first);
    for(uint32_t i=0;i<1024;i++) {
        if(!particles_buffer_0[i].valid || !particles_buffer_1[i].valid) continue;
        extern float interpolation_tick_progress;
        interpolate_vector(particles_buffer_1[i].position, particles_buffer_0[i].position, live_particles[i].position, interpolation_tick_progress);
    }
}

void on_particle_physics_before() noexcept {
    auto &particle_table = get_particle_table();
    auto *live_particles = reinterpret_cast<Particle *>(particle_table.first);
    for(uint32_t i=0;i<1024;i++) {
        if(!particles_buffer_0[i].valid) continue;
        live_particles[i].position = particles_buffer_0[i].position;
    }
    memcpy(particles_buffer_1,particles_buffer_0,sizeof(particles_buffer_0));
}

void on_particle_physics_after() noexcept {
    auto &particle_table = get_particle_table();
    auto *live_particles = reinterpret_cast<Particle *>(particle_table.first);
    for(uint32_t i=0;i<1024;i++) {
        particles_buffer_intermediate[i].position = live_particles[i].position;
    }
}
