/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicLong;
import net.jcip.annotations.ThreadSafe;

import factorizer.Factorizer;
import factorizer.IFactorizerService;


@ThreadSafe
public class FactorizerCounting implements IFactorizerService {
  private final AtomicLong count = new AtomicLong(0);

  public long getCount() {
    return count.get();
  }


  public ArrayList<Long> fact(long num) {
    ArrayList<Long> factors = Factorizer.fact(num);
    count.incrementAndGet();
    return factors;
  }
}

