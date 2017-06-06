#include <iostream>
#include <map>
#include <cstdio>
#include <cstring>
#include <benchmark/benchmark.h>

using namespace std;

template <typename K, typename V>
ostream& operator << (ostream& os, const map<K,V>& m){
  for (const auto& kv: m){
    os << kv.first << "-> " << kv.second << ", ";
  }
  os << endl;
}

static int recv_header(void *ptr, size_t len, map <string, string>& out_headers){
    int r;
    int http_status;
    char line[len+1];

    char *s = (char *)ptr, *end = (char *)ptr + len;
    char *p = line;
    //    cout << *s << *end << endl;
    while (s != end) {
      if (*s == '\r') {
        s++;
        continue;
      }
      if (*s == '\n') {
        *p = '\0';
        // TODO: fill whatever data required here
        char *l = line;
        char *tok = strsep(&l, " \t:");
        if (tok && l) {
          while (*l == ' ')
            l++;

          if (strcmp(tok, "HTTP") == 0 || strncmp(tok, "HTTP/", 5) == 0) {
            int http_status = atoi(l);
            if (http_status == 100) /* 100-continue response */
              continue;
          } else {
            /* convert header field name to upper case  */
            char *src = tok;
            char buf[len + 1];
            size_t i;
            for (i = 0; i < len && *src; ++i, ++src) {
              switch (*src) {
              case '-':
                buf[i] = '_';
                break;
              default:
                buf[i] = toupper(*src);
              }
            }
            buf[i] = '\0';
            out_headers[buf] = l;
          }
        }
      }
      if (s != end)
        *p++ = *s++;
    }


    return 0;

}

static void BM_recv_header(benchmark::State& state){
  // char cstr3[] = "HTTP 200\r\n";
  // char cstr2[] = "HTTP/1.1 FO\r\n";
  // char cstr1[] = "x-amz-version 100\r\n";
  // char cstr4[] = "x-amz-foo: bar\r\n";
  map <string,string> out_headers;
  size_t key_size = state.range(0);
  size_t str_size = 2*key_size+3; // key_size + space + val_size + \r\n
  char *src = new char[str_size];
  memset(src, 'x', key_size);
  src[key_size]= ':';
  memset(src+key_size+1,'y',key_size);
  src[2*key_size+1]='\r';
  src[2*key_size+2]='\n';
  while(state.KeepRunning()){
    recv_header(src, str_size, out_headers);
  }
}

BENCHMARK(BM_recv_header)->RangeMultiplier(2)->Range(8,8<<10);

BENCHMARK_MAIN();
