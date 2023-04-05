type
  List
  {
    int data;
    List next;
  }
endtype

decl
    List head;
enddecl

int main()
{
  decl
    int length, x;
    List p, q, curr, prev, next;
  enddecl

  begin
    initialize();

    head=null;
    read(length);

    q=head;
    while (length!=0)  do
      read(x);
      p=alloc();
      p.data=x;
      p.next=null;

      if (head==null) then
        head=p;
        q=p;
      else
        q.next=p;
        q=q.next;
      endif;

      length=length-1;
    endwhile;

    curr = head;
    prev = null;
    next = null;
    while(curr != null) do
      next = curr.next;
      curr.next = prev;
      prev = curr;
      curr = next;
    endwhile;
    head = prev;

    p=head;
    while(p!=null)  do
      write(p.data);
      p=p.next;
    endwhile;

    return 1;
  end
}
