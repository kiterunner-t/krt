/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.ArrayList;
import net.jcip.annotations.ThreadSafe;


@ThreadSafe
public class Factorizer {
  public static ArrayList<Long> fact(long n) {
    ArrayList<Long> factors = new ArrayList<Long>();
    long num = n;
    long key = 1;

    while (num > 1) {
      for (long i = 2; i <= num; i++) {
        if (num%i == 0) {
          key = i;
          factors.add(i);
          break;
        }
      }

      num /= key;
    }

    return factors;
  }
}

