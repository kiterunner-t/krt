/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.ArrayList;
import net.jcip.annotations.NotThreadSafe;

import factorizer.Factorizer;
import factorizer.IFactorizerService;


@NotThreadSafe
public class FactorizerUnsafeCounting implements IFactorizerService {
  private long count = 0;

  public long getCount() {
    return count;
  }


  public ArrayList<Long> fact(long num) {
    ArrayList<Long> factors = Factorizer.fact(num);
    ++count;
    return factors;
  }
}

