/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.concurrent.atomic.AtomicReference;
import java.util.ArrayList;
import net.jcip.anotations.NotThreadSafe;

import factorizer.IFactorizerService;
import factorizer.Factorizer;


@NotThreadSafe
public class FactorizerServiceUnsafeCache implements IFactorizerService {
  private final AtomicReference<Long> lastNumber
      = new AtomicReference<Long>();

  private final AtomicReference<ArrayList<Long>> lastFactors
      = new AtomicReference<ArrayList<Long>>();

  public FactorizerUnsafeCaching() {
    lastNumber.set(2L);
    ArrayList<Long> twoFactors = new ArrayList<Long>();
    twoFactors.add(2L);
    lastFactors.set(twoFactors);
  }


  public ArrayList<Long> fact(long num) {
    if (num == lastNumber.get()) {
      return lastFactors.get();

    } else {
      ArrayList<Long> factors = Factorizer.fact(num);
      lastNumber.set(num);
      lastFactors.set(factors);
      return factors;
    }
  }
}
