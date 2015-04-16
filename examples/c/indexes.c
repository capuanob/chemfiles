#include <stdio.h>
#include <stdlib.h>

#include "chemharp.h"

int main() {
    CHRP_TRAJECTORY* traj = chrp_open("tests/files/xyz/helium.xyz", "r");
    CHRP_FRAME* frame = NULL;

    if (traj == NULL)
        goto error;

    if (!chrp_trajectory_read_next(traj, frame) || frame == NULL)
        goto error;

    size_t natoms = 0;
    chrp_frame_size(frame, &natoms);

    float (*positions)[3] = (float(*)[3])malloc(sizeof(float[natoms][3]));
    unsigned* indexes = (unsigned*)malloc(sizeof(unsigned[natoms]));
    if (positions == NULL || indexes == NULL)
        goto error;

    for (int i=0; i<natoms; i++) {
        indexes[i] = (unsigned)-1;
    }

    if (!chrp_frame_positions(frame, positions, natoms))
        goto error;

    unsigned last_index = 0;
    for (int i=0; i<natoms; i++) {
        if (positions[i][0] < 5) {
            indexes[last_index] = i;
            last_index++;
        }
    }

    printf("Atoms with x < 5:\n");
    int i = 0;
    while(indexes[i] != (unsigned)-1 && i < natoms) {
        printf("  - %d\n", indexes[i]);
        i++;
    }
    printf("Number of atoms: %d\n", i);

    chrp_trajectory_close(traj);
    chrp_frame_free(frame);
    return 0;

error:
    printf("Error, cleaning up …\n");
    chrp_trajectory_close(traj);
    chrp_frame_free(frame);
    return 1;
}