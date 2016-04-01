int gcd(int a, int b) {
  //b is the GCD
  int r = a % b;
  if(r == 0)
    return b;
  else
    return gcd(b,r);
}

bool palindrome(const string &s) {
  if(s.size() <= 1) return true;
  if(s[0] != s[s.size() - 1])
    return false;
  palindrome(substr)
}

void solveParade(int n) {
  if(n == 1) return 2;
  if(n == 2) return 3;
}
