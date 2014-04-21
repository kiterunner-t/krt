import java.util.concurrent.atomic.AtomicLong;


public class NoVisibility {
  private final static long TEST_NUMBER_INIT_VALUE = 0L;
  private final static long TEST_NUMBER = 0x1000ffffffffL;

  private static boolean ready;
  private static long number;
  private static AtomicLong count = new AtomicLong(0);
  private static AtomicLong zeroCount = new AtomicLong(0);
  private static AtomicLong otherCount = new AtomicLong(0);


  private static class ReaderThread extends Thread {
    public void run() {
      while (!ready)
        Thread.yield();

      if (number == TEST_NUMBER_INIT_VALUE)
        zeroCount.getAndIncrement();
      else if (number == TEST_NUMBER)
        count.getAndIncrement();
      else
        otherCount.getAndIncrement();
    }
  }


  private static class WriterThread extends Thread {
    public void run() {
      number = TEST_NUMBER;
      ready = true;
    }
  }


  public static void main(String[] args) {
    for (int i = 0; i < 10000; ++i) {
      new ReaderThread().start();
      new WriterThread().start();

      number = TEST_NUMBER_INIT_VALUE;
      ready = false;
    }

    System.out.println("     count is: " + count.get());
    System.out.println(" zeroCount is: " + zeroCount.get());
    System.out.println("otherCount is: " + otherCount.get());
  }
}

