// baseado no codigo encontrado em 2.2.5 do livro Compilers: Principles, Techniques and Tools
import java.io.*;

class Parser{
  static int lookahead;

  public Parser() throws IOException{
    lookahead = System.in.read();
  }

  void expr() throws IOException {
    term();
    while (true) {
      switch (lookahead) {
        case '+':
          match('+'); term(); System.out.write('+');
          break;

        case '-':
          match('-'); term(); System.out.write('-');
          break;

        default:
          return;
      }
    }
  }

  void term() throws IOException {
    if (Character.isDigit(lookahead)){ 
      System.out.write(lookahead);
      lookahead = System.in.read();
    }
    else throw new Error("syntax error");
  }

  void match(int c) throws IOException {
    if (lookahead == c) lookahead = System.in.read();
    else throw new Error("syntax error");
  }
}

public class Postfix {
  public static void main(String[] args) throws IOException {
    Parser parse = new Parser();
    parse.expr(); 
    System.out.write('\n');
  }
}

