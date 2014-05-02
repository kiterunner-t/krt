/**
 * Copyleft (C) KRT, 2014 by kiterunner_t
 */

package factorizer;

import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

import factorizer.Factorizer;
import factorizer.IFactorizerService;


public class TestFactorizerService {
  private static final AtomicInteger counts = new AtomicInteger();
  private static final AtomicInteger errorCounts = new AtomicInteger();


  public static void equal(String msg, boolean cond) {
    counts.addAndGet(1);
    if (cond == false) {
      errorCounts.addAndGet(1);
      System.out.println("ERROR TEST " + msg);
    }
    // System.out.println("test ok");
  }


  public static void main(String[] args) {
    int runTimesPerThread = 1000;
    int threadNums = 3;
    int maxRandom = 1000000;
    long start = System.currentTimeMillis();

    switch (args.length) {
    case 3: maxRandom = Integer.parseInt(args[2]);
    case 2: runTimesPerThread = Integer.parseInt(args[1]);
    case 1: threadNums = Integer.parseInt(args[0]);
    }

    for (int j = 0; j < runTimesPerThread; ++j) {
      Random random = new Random(System.currentTimeMillis());
      ArrayList<Thread> threads = new ArrayList<Thread>();

      for (int i = 0; i < threadNums; ++i) {
        threads.add(new Thread(new FactorizerWorker(random.nextLong() % maxRandom)));
      }

      for (int i = 0; i < threadNums; ++i) {
        threads.get(i).start();
      }

      try {
        for (int i = 0; i < threadNums; ++i) {
          threads.get(i).join();
        }
      } catch (Exception e) {
        // ...
      }
    }

    System.out.println("\n           time: " + (System.currentTimeMillis() - start));
    System.out.println("  all counts is: " + counts);
    System.out.println("error counts is: " + errorCounts);
  }
}


class FactorizerWorker implements Runnable {
  private final long num;
  private final ArrayList<Long> precalcFactors;

  public FactorizerWorker(long num) {
    this.num = num;
    this.precalcFactors = Factorizer.fact(num);
  }


  private void compare(ArrayList<Long> factors) {
    int size = precalcFactors.size();
    TestFactorizerService.equal("factor size error", size == factors.size());
    for (int i = 0; i < size; ++i) {
      TestFactorizerService.equal("factor element error",
          precalcFactors.get(i).compareTo(factors.get(i)) == 0);
    }
  }


  public void run() {
    IFactorizerService fact = new FactorizerServiceStateless();
    ArrayList<Long> factors = fact.fact(num);
    compare(factors);
  }
}

