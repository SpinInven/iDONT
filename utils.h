#ifndef UTILS_H
#define UTILS_H
int has_passed(unsigned int start, unsigned int duration)
{
  return millis() - start > duration;
}
#endif
