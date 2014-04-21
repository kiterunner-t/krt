@ThreadSafe
public class StatelessFactorizer implements Servlet {
  public void service(ServeletRequest req, ServletResponse resp) {
    BigInteger i = extractFromRequest(req);
    BigInteger[] factors = factor(i);
    encodeIntoResponse(resp, factors);
  }
}

