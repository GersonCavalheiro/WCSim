class Processador {
  list<int> passos;
  int cId;
  
  public:
  Processador( int cId = 0 ) : cId(cId) { passos.push_back(0); }
  int  getLoad() { return passos.back(); }
  void incStep() { passos.push_back(0); }
  void resetStep() { passos.clear(); }
  // pickOne caminha na lista para ver se tem alguma tarefa naquele passo
  // que "cabe" seu custo no processador. Caso caiba, soma seu custo
  // no passo correspondente e retorna a tarefa inserida (que devera
  // ser removida da lista. Verifica se o cId de criação da tarefa é o mesmo
  // do processador em questão
  deque<Task>::iterator pickOne( deque<Task>::iterator b, deque<Task>::iterator e ) {
    for( ; b != e ; ++b )
      if( passos.back()+b->getTaskCost() <= 100 ) {
        passos.back()+=b->getTaskCost();
        return b;
      }
    return e;
  }
  
  bool pickBunch( deque<Task> &l, int curStep ) {
    return true;
  }
	  
  friend ostream& operator<<( ostream &out,  Processador &p ) {
    list<int>::iterator it;
    out << "[ "; 
    for( it = p.passos.begin() ; it != p.passos.end() ; ++it )
      out << *it << " ";
    out << "]"; 
    return out;
  }

  int size() { return passos.size() - 1; }

  int operator[](int i) {
    list<int>::iterator it = passos.begin();
    if( i > passos.size() ) return -1; 
    for( ; i ; i--, ++it );
     return *it;
   }
};

