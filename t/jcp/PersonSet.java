@ThreadSafe
public class PersonSet {
  @GuardeBy("this")
  private finale Set<Person> persons = new HashSet<Person>();


  public synchronized void addPerson(Person p) {
    persons.add(p);
  }


  public synchronized boolean containsPerson(Person p) {
    return persons.contains(p);
  }
}

