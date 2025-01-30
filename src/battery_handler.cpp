#include "battery_handler.h"

BatteryHandler::BatteryHandler() {}
BatteryHandler::~BatteryHandler() {}

void BatteryHandler::init(const BatteryHandlerConfig &config)
{
    m_mav_bridge = config.mav_bridge;
    uint8_t num_cells = auto_detect_num_cells();
    m_check_interval = config.check_interval;
    m_low_voltage_threshold = config.low_voltage_threshold * num_cells;
    m_low_voltage_timeout = config.low_voltage_timeout;
    m_critical_voltage_threshold = config.critical_voltage_threshold * num_cells;
    m_critical_voltage_timeout = config.critical_voltage_timeout;
    m_check_timer.start();
    m_low_voltage_timer.start();
    m_critical_voltage_timer.start();
}

void BatteryHandler::run()
{
    if (m_check_timer.hasPassed(m_check_interval, true))
    {
        check_voltage();
        update_battery_status();
    }
}

void BatteryHandler::check_voltage()
{
    voltage = m_mav_bridge->get_mavlink_data().battery_voltage;
    bool usb_power = voltage < 5.5;
    if (voltage > m_low_voltage_threshold || usb_power)
    {
        m_low_voltage_timer.restart();
    }
    if (voltage > m_critical_voltage_threshold || usb_power)
    {
        m_critical_voltage_timer.restart();
    }
}

void BatteryHandler::update_battery_status()
{
    if (m_critical_voltage_timer.hasPassed(m_critical_voltage_timeout))
    {
        m_battery_status = BATTERY_CRITICAL;
    }
    else if (m_low_voltage_timer.hasPassed(m_low_voltage_timeout))
    {
        m_battery_status = BATTERY_LOW;
    }
    else
    {
        m_battery_status = BATTERY_NORMAL;
    }
}

uint8_t BatteryHandler::auto_detect_num_cells()
{
    float tmp_voltage = m_mav_bridge->get_mavlink_data().battery_voltage;
    if (tmp_voltage == 0)
    {
        delay(1000);
        tmp_voltage = m_mav_bridge->get_mavlink_data().battery_voltage;
    }
    return tmp_voltage > 4 * 4.3 ? 6 : 4;
}
BatteryStatus BatteryHandler::get_battery_status()
{
    return m_battery_status;
}