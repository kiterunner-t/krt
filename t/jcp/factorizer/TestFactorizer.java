/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.ArrayList;
import java.util.Scanner;

import factorizer.Factorizer;
import factorizer.IFactorizerService;


public class TestFactorizer {
  public static void main(String[] args) {
    IFactorizerService fact = new FactorizerServiceUnsafeCache();

    while (true) {
      Scanner input = new Scanner(System.in);
      System.out.print("input an integer: ");
      long n = input.nextLong();
      if (n <= 1) {
        break;
      }

      ArrayList<Long> factors = fact.fact(n);
      int len = factors.size();
      for (int i = 0; i < len; i++) {
        System.out.println(factors.get(i));
      }
    }
  }
}

