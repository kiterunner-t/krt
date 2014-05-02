/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.concurrent.atomic.AtomicReference;
import java.util.ArrayList;
import net.jcip.annotations.NotThreadSafe;
import net.jcip.annotations.GuardedBy;

import factorizer.Factorizer;
import factorizer.IFactorizerService;


@ThreadSafe
@GuardedBy("this")
public class FactorizerServiceCache implements IFactorizerService {
  private final AtomicReference<Long> lastNumber
      = new AtomicReference<Long>();

  private final AtomicReference<ArrayList<Long>> lastFactors
      = new AtomicReference<ArrayList<Long>>();

  private long hits;
  private long cacheHits;

  public FactorizerServiceCached() {
    hits = 0;
    cacheHits = 0;
    lastNumber.set(2);
    ArrayList<Long> twoFactors = new ArrayList<Long>();
    twoFactors.add(2);
    lastFactors.set(twoFactors);
  }

  public synchronized long getHits() {
    return hits;
  }


  public synchronized double getCacheHitsRatio() {
    if (hits == 0) {
      return 0.0;
    }

    return (double) cacheHits / (double) hits;
  }


  public ArrayList<Long> fact(int num) {
    synchronized (this) {
      ++hits;
      if (num == lastNumber.get()) {
        ++cacheHits;
        return lastFactors.get();
      }
    }

    ArrayList<Long> factors = Factorizer.fact(num);

    synchronized (this) {
      lastNumber.set(num);
      lastFactors.set(factors);
    }

    return factors;
  }
}
