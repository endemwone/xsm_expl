type
  bst{
  int a;
  bst left;
  bst right;
  }
endtype

decl
  int in,opt;
  bst insert(bst h, int key);
  int inOrder(bst h);
  int preOrder(bst h);
  int postOrder(bst h);
enddecl

bst insert(bst h, int key)
{

  begin
    if (h == null) then
    h = alloc();
    h.a = key;
    h.left = null;
    h.right = null;

    else if (key < h.a) then
            h.left  = insert(h.left, key);

         else if (key > h.a) then
                 h.right = insert(h.right, key);
              endif;
          endif;
    endif;

    return h;
  end
}

int inOrder(bst h){

  begin

  if(h!=null) then

    in=inOrder(h.left);
    write(h.a);
    in=inOrder(h.right);
  endif;
  return 1;
  end
}

int preOrder(bst h){

  begin

  if(h!=null) then
    write(h.a);
    in=preOrder(h.left);

    in=preOrder(h.right);
  endif;
  return 1;
  end
}

int postOrder(bst h){

  begin

  if(h!=null) then

    in=postOrder(h.left);

    in=postOrder(h.right);
    write(h.a);
  endif;
  return 1;
  end
}

int main()
{
  decl
  int val,flag;
  bst Root;
  enddecl

  begin
    initialize();
    Root = null;

    read(val);

    while(val!=0) do
      Root = insert(Root,val);
      read(val);
    endwhile;

    in = inOrder(Root);
    in = preOrder(Root);
    in = postOrder(Root);

  return 9;
  end
}
