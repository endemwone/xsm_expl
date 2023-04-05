class
    A
    {
        decl
            int i;
            int f0();
            int f1();
        enddecl
        int f0() {
            decl
                int c;
            enddecl
            begin
                c = self.f1();
                write(self.i);
                return 1;
            end
        }
        int f1() {
            decl
            enddecl
            begin
                self.i=0;
                write("In A F1");
                return 1;
            end
        }
    }
    B extends A
    {
        decl
            int f1();
        enddecl
        int f1() {
            decl
            enddecl
            begin
                self.i=1;
                write("In B F1");
                return 1;
            end
        }
    }
endclass
decl
    int n;
    A obj;
enddecl
int main() {
    decl
    enddecl
    begin
        initialize();
        read(n);
        if(n>0) then
            obj = new(A);
        else
            obj = new(B);
        endif;
        n = obj.f0();
        return 1;
    end
}
