@ThreadSafe
public final class Counter {
  @GuardedBy("this") private long value = 0;


  public synchronized long get() {
    return value;
  }


  public synchronized long increment() {
    if (value == Long.MAX_VALUE)
      throw new IllegalStateException("counter overflow");

    return ++value;
  }
}

