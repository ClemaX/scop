#include <stdlib.h>
#include <string.h>

#include <face.h>

void	face_destroy(face *face)
{
	free(face->elems);
	bzero(face, sizeof(*face));
}
