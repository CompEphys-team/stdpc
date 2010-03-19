
template<class T>
void getEntry(T &target, const T source, bool &change)
{
  T tmp;
  
  tmp= source;
  if (tmp != target) {
    target= tmp;
    change= true;
  }
} 
