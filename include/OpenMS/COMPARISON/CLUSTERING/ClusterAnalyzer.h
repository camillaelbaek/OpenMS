// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//

//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Mathias Walzer $
// --------------------------------------------------------------------------
//
#ifndef OPENMS_COMPARISON_CLUSTERING_CLUSTERANALYZER_H
#define OPENMS_COMPARISON_CLUSTERING_CLUSTERANALYZER_H

#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/DATASTRUCTURES/DistanceMatrix.h>
#include <OpenMS/CONCEPT/Exception.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <cfloat>

namespace OpenMS
{


	/** @brief Elements of a binary tree used to represent a hierarchical clustering process

			strict indexing/topology is assumed, i.e. node no. x represents clusteringstep no. x
			left_child and right_child are indices to the clusters merged (not to the elements), distance is the distance of the two children
	*/
	class OPENMS_DLLAPI BinaryTreeNode
	{
		public:
		/// constructor
		BinaryTreeNode(size_t i, size_t j, Real x);

		/// destructor
		~BinaryTreeNode();

		/// copy constructor
		BinaryTreeNode(const BinaryTreeNode& source);

		/// assignment operator
		BinaryTreeNode& operator = (const BinaryTreeNode& source);

		size_t left_child;
		size_t right_child;
		Real distance;

		private:
		BinaryTreeNode();
	};


	/**
		@brief Bundles analyzing tools for a clustering (given as sequence of BinaryTreeNode's)

		@ingroup SpectraClustering
	*/
	class OPENMS_DLLAPI ClusterAnalyzer
	{
	public:
	/// default constructor
	ClusterAnalyzer();

	/// copy constructor
	ClusterAnalyzer(const ClusterAnalyzer& source);

	/// destructor
	virtual ~ClusterAnalyzer();

	/**
		@brief Method to calculate the average silhouette widths for a clustering

		@param tree vector of BinaryTreeNode's representing the clustering
		@param original DistanceMatrix for all clustered elements started from
		@return a vector filled with the average silhouette widths for each clusterstep

		The average silhouette width will becalculated for each clustering step beginning with the first step(n-1 cluster) ending with the last (1 cluster, average silhouette width is 0 by definition).
		@see BinaryTreeNode
	*/
	std::vector< Real > averageSilhouetteWidth(std::vector<BinaryTreeNode>& tree, DistanceMatrix<Real>& original);

	/**
		@brief Method to calculate Dunns indices for a clustering

		@param tree vector of BinaryTreeNode's representing the clustering
		@param original DistanceMatrix for all clustered elements started from
		@param tree_from_singlelinkage true if tree was created by SingleLinkage, i.e. the distances are the minimal distances in increasing order and can be used to speed up the calculation
		@see BinaryTreeNode
	*/
	std::vector< Real > dunnIndices(std::vector<BinaryTreeNode>& tree, DistanceMatrix<Real>& original, bool tree_from_singlelinkage = false);

	/**
		@brief Method to calculate the cohesions of a certain partition

		@param clusters vector of vectors holding the clusters (with indices to the actual elements)
		@param original DistanceMatrix for all clustered elements started from
		@return a vector that holds the cohesions of each cluster given with @p clusters (order corresponds to @p clusters)
	*/
	std::vector< Real > cohesion(std::vector< std::vector<UInt> >& clusters, DistanceMatrix<Real>& original);

	/**
		@brief Method to calculate the average aberration from average population in partition resulting from a certain step in clustering

		@param cluster_quantity desired partition size analogue to ClusterAnalyzer::cut
		@param tree vector of BinaryTreeNode's representing the clustering
		@throw invalid_parameter if desired clustering is invalid
		@return the average aberration from the average cluster population (number of elements/cluster_quantity) at cluster_quantity
		@see BinaryTreeNode
	*/
	Real averagePopulationAberration(UInt cluster_quantity, std::vector<BinaryTreeNode>& tree);

/**
		@brief Method to calculate a partition resulting from a certain step in clustering given by the number of clusters

		@param cluster_quantity size_t giving the number of clusters (i.e. starting elements - cluster_quantity = cluster step)
		@param tree vector of BinaryTreeNode's representing the clustering
		@param clusters vector of vectors holding the clusters (with indices to the actual elements)
		@param original DistanceMatrix for all clustered elements started from
		@throw invalid_parameter if desired clusterstep is invalid
		@see BinaryTreeNode

		after call of this method the argument clusters is filled corresponding to the given @p cluster_quantity with the indices of the elements clustered
	*/
	void cut(size_t cluster_quantity, std::vector< std::vector<UInt> >& clusters, std::vector<BinaryTreeNode>& tree);

/**
		@brief Returns the hirarchy described by a clustering tree as Newick-String

		@param tree vector of BinaryTreeNode's representing the clustering
		@param include_distance bool value indicating whether the distance shall be included to the string
		@see BinaryTreeNode

	*/
	String newickTree(std::vector<BinaryTreeNode>& tree, bool include_distance = false);

	private:
	/// assignment operator
	ClusterAnalyzer& operator = (const ClusterAnalyzer& source);

	};
	///returns the value of (x.distance < y.distance) for use with sort
	bool compareBinaryTreeNode(const BinaryTreeNode& x, const BinaryTreeNode& y);
}
#endif //OPENMS_COMPARISON_CLUSTERING_CLUSTERANALYZER_H
