/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.ArrayList;

import factorizer.IFactorizerService;
import factorizer.Factorizer;


public class FactorizerServiceStateless implements IFactorizerService {
  public ArrayList<Long> fact(long num) {
    return Factorizer.fact(num);
  }
}

