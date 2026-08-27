package at.zimo.klug;

/**
 * Represents a single firmware within a {@link at.zimo.klug.ZSU} file. Methods use the {@link [at.zimo.klug.KLUGAdapter]} 
 * API if applicable and an instance only holds constant identifiers of a firmware.
 */
public class Firmware {
  /**
   * Construct a firmware
   * 
   * @param zsu     The {@link at.zimo.klug.ZSU} file this firmware is part of
   * @param index   The index within the {@link at.zimo.klug.ZSU} file
   */
  Firmware(ZSU zsu, int index) {
    _zsu = zsu; 
    _index = index;
  }

  /**
   * Returns the firmware ID (matching the compatible decoder ID)
   * 
   * @return Firmware ID
   * 
   * @throws IllegalStateException If the {@link at.zimo.klug.ZSU} was closed
   */
  public long id() {
    return KLUGAdapter.zsuGetFirmwareId(_zsu.get(), _index);
  }

  /**
   * Returns the firmware name (matching the compatibel decoder name e.g. MS450)
   * 
   * @return Firmware Name
   * 
   * @throws IllegalStateException If the {@link at.zimo.klug.ZSU} was closed
   */
  public String name() {
    return KLUGAdapter.zsuGetFirmwareName(_zsu.get(), _index);
  }

  /**
   * Returns the bootloader type (Only needed for MX-Decoders)
   * 
   * @return Bootloader type
   * 
   * @throws IllegalStateException If the {@link at.zimo.klug.ZSU} was closed
   */
  public int type() {
    return KLUGAdapter.zsuGetFirmwareType(_zsu.get(), _index);
  }

  /**
   * Returns the number of flash blocks within the firmware
   * 
   * @return Flash block count
   * 
   * @throws IllegalStateException If the {@link at.zimo.klug.ZSU} was closed
   */
  public int blocks() {
    return KLUGAdapter.zsuGetFirmwareBlockCount(_zsu.get(), _index);
  }

  /**
   * Returns the firmware major version   
   * 
   * @return Major version
   *  
   * @throws IllegalStateException If the {@link at.zimo.klug.ZSU} was closed
   */
  public String versionMajor() {
    return KLUGAdapter.zsuGetFirmwareMajorVersion(_zsu.get(), _index);
  }

  /**
   * Returns the firmware minor version
   * 
   * @return Minor version
   * 
   * @throws IllegalStateException If the {@link at.zimo.klug.ZSU} was closed
   */
  public String versionMinor() {
    return KLUGAdapter.zsuGetFirmwareMinorVersion(_zsu.get(), _index);
  }

  /**
   * Returns the index of this firmware within the {@link at.zimo.klug.ZSU} file
   * 
   * @return Index
   */
  public int index() {
    return _index;
  }

  /**
   * Returns the holding {@link at.zimo.klug.ZSU} file
   * 
   * @return zsu
   */
  public ZSU zsu() {
    return _zsu;
  }

  private final ZSU _zsu; 
  private final int _index; 
}
