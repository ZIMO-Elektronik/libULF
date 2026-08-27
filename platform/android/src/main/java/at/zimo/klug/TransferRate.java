package at.zimo.klug;

/**
 * MDU Transfer rate enum
 */
public enum TransferRate {
  Fallback(0), 
  Fast(1),
  Medium(2),
  Slow(3), 
  Default(4);
  
  TransferRate(int value) {
    if (value < 0 || value > 4) throw new IllegalArgumentException("Value must be between 0 and 4");
    this._value = value; 
  }
  
  public int value() {
    return _value; 
  }
  private final int _value;
}