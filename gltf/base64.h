#ifndef __DINO3_BASE64_H__
#define __DINO3_BASE64_H__

// Modified base64 encoder and decoder from https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

#include <string>

inline std::string base64_encode(const std::string &in) {
  
  std::string out;
  
  unsigned int val=0;
  int valb=-6;
  for (unsigned char c : in) {
    val = (val<<8) + c;
    valb += 8;
    while (valb>=0) {
      out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
      valb-=6;
    }
  }
  if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
  while (out.size()%4) out.push_back('=');
  return out;
}

template<class ITERATOR>
std::string base64_decode(ITERATOR begin, ITERATOR end)
{
  
  std::string out;

  std::vector<int> T(256,-1);
  for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 
  
  unsigned int val=0;
  int valb=-8;
  while(begin != end)
  {
    if (T[*begin] == -1) break;
    val = (val<<6) + T[*begin];
    valb += 6;
    if (valb>=0) {
      out.push_back(char((val>>valb)&0xFF));
      valb-=8;
    }
    ++begin;
  }
  return out;
}

#endif
