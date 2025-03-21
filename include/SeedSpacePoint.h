#ifndef SEEDSPACEPOINT_H
#define SEEDSPACEPOINT_H 1

#include <Acts/Definitions/Algebra.hpp>
#include <Acts/Definitions/Common.hpp>
#include <cmath>
#include <vector>

namespace MarlinACTS {

class SeedSpacePoint
{
public:

    template <typename position_t>
    SeedSpacePoint(const Eigen::MatrixBase<position_t>& pos, float varRho,
                   float varZ, const SourceLink& sourceLink) :
        m_x(pos[Acts::ePos0]),
        m_y(pos[Acts::ePos1]),
        m_z(pos[Acts::ePos2]),
        m_rho(std::hypot(m_x, m_y)),
        m_varianceRho(varRho),
        m_varianceZ(varZ),
        m_sourceLink(sourceLink)
    {
        EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(position_t, 3);
    }

    constexpr float x() const { return m_x; }
    constexpr float y() const { return m_y; }
    constexpr float z() const { return m_z; }
    constexpr float r() const { return m_rho; }
    constexpr float varianceR() const { return m_varianceRho; }
    constexpr float varianceZ() const { return m_varianceZ; }

    constexpr SourceLink sourceLink() const { return m_sourceLink; }

    const std::optional<float> t() const
    {
        return m_sourceLink.lciohit()->getTime();
    }
    // TODO missing: const std::optional<float> varianceT() const

private:
    // Global position
    float m_x;
    float m_y;
    float m_z;
    float m_rho;
    // Variance in rho/z of the global coordinates
    float m_varianceRho;
    float m_varianceZ;
    // Index of the corresponding measurement
    SourceLink m_sourceLink;
};

constexpr bool operator==(const SeedSpacePoint& lhs,
                          const SeedSpacePoint& rhs)
{
    // TODO would it be sufficient to check just the index under the assumption
    //   that the same measurement index always produces the same space point?
    // no need to check r since it is fully defined by x/y
    return (lhs.sourceLink() == rhs.sourceLink()) and (lhs.x() == rhs.x()) and
        (lhs.y() == rhs.y()) and (lhs.z() == rhs.z()) and
        (lhs.varianceR() == rhs.varianceR()) and
        (lhs.varianceZ() == rhs.varianceZ());
}

using SeedSpacePointContainer = std::vector<SeedSpacePoint>;

}  // namespace MarlinACTS

#endif //SEEDSPACEPOINT_H
