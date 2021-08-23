#ifndef DISPLAYS_H
#define DISPLAYS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DISPLAYS 10

#if defined(_DISPLAYS_WIN32)
extern void get_all(void **displays, int *length);
extern void get_values(void *displays, int index, int *x, int *y, int *width, int *height, int *is_default, void **internal);
extern void get_default(int *x, int *y, int *width, int *height, int *is_default, void **internal);
extern void get_index(int *index, void *internal);
extern void free_memory(void *displays);
#elif defined(_DISPLAYS_X)
/* TODO */
#endif

#ifdef __cplusplus
}
#endif

#endif /* DISPLAYS_H */