#ifndef MISC_H__
#define MISC_H__
bool isJSON(String data) {
  return data[0] == '{' && data[data.length() - 1] == '}';
}
#endif