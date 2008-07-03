/* Copyright (c) 2008, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

package java.util;

public class TreeSet<T>  extends AbstractSet<T> implements Collection<T> {
  private PersistentSet<Cell<T>> set;
  private int size;

  public TreeSet(final Comparator<T> comparator) {
    set = new PersistentSet(new Comparator<Cell<T>>() {
      public int compare(Cell<T> a, Cell<T> b) {
        return comparator.compare(a.value, b.value);
      }
    });
    size = 0;
  }

  public Iterator<T> iterator() {
    return new MyIterator<T>(set.first());
  }

  public String toString() {
    return Collections.toString(this);
  }

  public boolean add(T value) {
    PersistentSet.Path<Cell<T>> p = set.find(new Cell(value, null));
    if (p.fresh()) {
      set = p.add();
      ++size;
      return true;
    }
    return false;
  }

  // Used by hashMaps for replacement
  public void addAndReplace(T value) {
    PersistentSet.Path<Cell<T>> p = set.find(new Cell(value, null));
    if (p.fresh()) {
      set = p.add();
      ++size;
    } else {
      set = p.replaceWith(new Cell(value, null));
    }
  }
    
  public boolean remove(T value) {
    PersistentSet.Path<Cell<T>> p = set.find(new Cell(value, null));
    if (p.fresh()) {
      return false;
    } else {
      --size;

      if (p.value().next != null) {
        set = p.replaceWith(p.value().next);
      } else {
        set = p.remove();
      }

      return true;
    }
  }

  public int size() {
    return size;
  }

  public boolean isEmpty() {
    return size == 0;
  }

  public boolean contains(T value) {
    return !set.find(new Cell(value, null)).fresh();
  }

  public void clear() {
    set = new PersistentSet(set.comparator());
    size = 0;
  }

  private class MyIterator<T> implements java.util.Iterator<T> {
    private PersistentSet.Path<Cell<T>> path;
    private PersistentSet.Path<Cell<T>> nextPath;
    private Cell<T> cell;
    private Cell<T> prevCell;
    private Cell<T> prevPrevCell;
    private boolean canRemove = false;

    private MyIterator(PersistentSet.Path<Cell<T>> path) {
      this.path = path;
      if (path != null) {
        cell = path.value();
        nextPath = path.successor();
      }
    }

    private MyIterator(MyIterator<T> start) {
      path = start.path;
      nextPath = start.nextPath;
      cell = start.cell;
      prevCell = start.prevCell;
      prevPrevCell = start.prevPrevCell;
      canRemove = start.canRemove;
    }

    public boolean hasNext() {
      return cell != null || nextPath != null;
    }

    public T next() {
      if (cell == null) {
        path = nextPath;
        nextPath = path.successor();
        cell = path.value();
      }
      prevPrevCell = prevCell;
      prevCell = cell;
      cell = cell.next;
      canRemove = true;
      return prevCell.value;
    }

    public void remove() {
      if (! canRemove) throw new IllegalStateException();

      --size;

      if (prevPrevCell != null && prevPrevCell.next == prevCell) {
        // cell to remove is not the first in the list.
        prevPrevCell.next = prevCell.next;
        prevCell = prevPrevCell;
      } else if (prevCell.next == cell && cell != null) {
        // cell to remove is the first in the list, but not the last.
        set = (PersistentSet) path.replaceWith(cell);
        prevCell = null;
      } else {
        // cell is alone in the list.
        set = (PersistentSet) path.remove();
        path = path.successor();
        if (path != null) {
          prevCell = null;
          cell = path.value();
          path = (PersistentSet.Path) set.find((Cell) cell);
        }
      }

      canRemove = false;
    }
  }    
}
