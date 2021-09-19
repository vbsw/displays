#ifndef DISPLAYS_H
#define DISPLAYS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DISPLAYS 10

extern void get_all(void **displays, int *length);
extern void get_values(void *displays, int index, int *x, int *y, int *width, int *height, int *is_default);
extern void get_default(int *x, int *y, int *width, int *height, int *index, int *is_default);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAYS_H */
