package at.zimo.klug;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * An iterator for the Firmware instances a {@link at.zimo.klug.ZSU} file holds
 */
public class FirmwareIterator implements Iterator<Firmware>{
    /**
     * Construct a FirmwareIterator
     */
    FirmwareIterator(ZSU zsu) {
      _zsu = zsu;
    }

    /**
     * Checks, if the iterator has a next firmware
     * 
     * @return true, if there is a next firmware
     */
    @Override
    public boolean hasNext() {
      return index < count; 
    }

    /**
     * Return the next firmware
     * 
     * @return next firmware
     * 
     * @throws NoSuchElementException If out of bounds
     */
    @Override
    public Firmware next() {
      if (!hasNext()) {
        throw new NoSuchElementException();
      }
      return _zsu.getFirmware(index++);
    }
  
    private final ZSU _zsu; 
    private int index = 0; 
    private final int count = 0; 
}
