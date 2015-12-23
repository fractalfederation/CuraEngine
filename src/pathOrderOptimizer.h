/** Copyright (C) 2013 David Braam - Released under terms of the AGPLv3 License */
#ifndef PATHOPTIMIZER_H
#define PATHOPTIMIZER_H

#include <stdint.h>
#include "utils/polygon.h"
#include "settings.h"

namespace cura {
 
/*!
 * Parts order optimization class.
 * 
 * Utility class for optimizing the path order by minimizing the distance traveled between printing different parts in the layer.
 * The order of polygons is optimized and the startingpoint within each polygon is chosen.
 */
class PathOrderOptimizer
{
public:
    EZSeamType type;
    Point startPoint; //!< The location of the nozzle before starting to print the current layer
    std::vector<PolygonRef> polygons; //!< the parts of the layer (in arbitrary order)
    std::vector<int> polyStart; //!< polygons[i][polyStart[i]] = point of polygon i which is to be the starting point in printing the polygon
    std::vector<int> polyOrder; //!< the optimized order as indices in #polygons

    PathOrderOptimizer(Point startPoint, EZSeamType type = EZSeamType::SHORTEST)
    : type(type)
    , startPoint(startPoint)
    {
    }

    void addPolygon(PolygonRef polygon)
    {
        this->polygons.push_back(polygon);
    }

    void addPolygons(Polygons& polygons)
    {
        for(unsigned int i=0;i<polygons.size(); i++)
            this->polygons.push_back(polygons[i]);
    }

    void optimize(); //!< sets #polyStart and #polyOrder

private:
    int getPolyStart(Point prev_point, int poly_idx);
    int getClosestPointInPolygon(Point prev, int i_polygon); //!< returns the index of the closest point
    int getFarthestPointInPolygon(int poly_idx); //!< return the index to the point farthest from the front (highest y)
    int getRandomPointInPolygon(int poly_idx);


};
//! Line path order optimization class.
/*!
* Utility class for optimizing the path order by minimizing the distance traveled between printing different lines within a part.
*/
class LineOrderOptimizer
{
    typedef typename std::vector<size_t> Cluster; //To make it more clear what a cluster is.

public:
    /*!
     * \brief The size of the grid cells used to cluster lines.
     * 
     * Increase this value to make the optimisation algorithm fall back to
     * nearest neighbour more often. Reduce this value to make the optimisation
     * algorithm use random insertion on smaller pieces of the input.
     */
    const unsigned long long cluster_grid_size;
    
    Point startPoint; //!< The location of the nozzle before starting to print the current layer
    std::vector<PolygonRef> lines; //!< the parts of the layer (in arbitrary order)
    std::vector<int> polyStart; //!< polygons[i][polyStart[i]] = point of polygon i which is to be the starting point in printing the polygon
    std::vector<int> polyOrder; //!< the optimized order as indices in #polygons

    /*!
     * \brief Constructs the line order optimiser with the specified settings.
     * 
     * \param start_point The starting point from where the paths generated by
     * this optimiser must start.
     * \param cluster_grid_size The size of the grid cells used to cluster
     * lines. Make this bigger and the optimiser will fall back to nearest
     * neighbour search more often. Make this smaller and the optimiser will
     * use random insertion more often.
     */
    LineOrderOptimizer(const Point& start_point, unsigned long long cluster_grid_size);

    void addPolygon(PolygonRef polygon)
    {
        this->lines.push_back(polygon);
    }

    void addPolygons(Polygons& polygons)
    {
        for(unsigned int i = 0; i < polygons.size(); i++)
        {
            this->lines.push_back(polygons[i]);
        }
    }

    void optimize(); //!< sets #polyStart and #polyOrder

private:
    /*!
     * \brief Clusters the polygons in groups such that the start and end of the
     * polygons in each group are close together.
     * 
     * This performs a simple nearest-neighbour traversal through all lines. An
     * arbitrary line is chosen as starting point for a cluster, and iteratively
     * the nearest neighbouring line will get added to that cluster. A line is
     * only neighbouring if both of its endpoints are nearby the endpoints of
     * the previous line. This way you get logical groups of lines that should
     * always be in sequence, with fairly low computational cost.
     * 
     * \return Clusters of polygons, where each cluster is represented with a
     * vector of indices pointing to positions in \link polygons.
     */
    std::vector<Cluster> cluster();

};

}//namespace cura

#endif//PATHOPTIMIZER_H
