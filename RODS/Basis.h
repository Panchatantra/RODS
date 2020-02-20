#pragma once

/**
 * @brief      The basic class of RODS
 */
class Basis
{
public:
    Basis();
    /**
     * @brief      Constructs a new instance.
     *
     * @param[in]  id    The identifier
     */
    Basis(const int id);
    ~Basis();

    int id; ///< The identifier
};