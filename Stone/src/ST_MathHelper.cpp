#include "ST_MathHelper.h"

std::random_device ST_MathHelper::m_RandomDevice;
std::mt19937 ST_MathHelper::m_Generator( m_RandomDevice() );