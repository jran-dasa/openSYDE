//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Helper class to create line bounding shape (implementation)

   Helper class to create line bounding shape

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     12.10.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <cmath>

#include "C_OSCUtils.h"
#include "C_GiBiLineBounding.h"

/* -- Used Namespaces ------------------------------------------------------ */

using namespace stw_types;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_core;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Constructor for multiple points

   \param[in] orc_Points                  Points to get bounding element for
   \param[in] orf64_Width                 Width of bounding element
   \param[in] orf64_InteractionPointWidth Interaction point width

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_GiBiLineBounding::C_GiBiLineBounding(const QVector<QPointF> & orc_Points, const float64 & orf64_Width,
                                       const float64 & orf64_InteractionPointWidth) :
   mc_Points(orc_Points),
   mf64_Width(orf64_Width),
   mf64_InteractionPointWidth(orf64_InteractionPointWidth)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Constructor for single line

   \param[in] orc_Line                    Lines to get bounding element for
   \param[in] orf64_Width                 Width of bounding element
   \param[in] orf64_InteractionPointWidth Interaction point width

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_GiBiLineBounding::C_GiBiLineBounding(const QLineF & orc_Line, const float64 & orf64_Width,
                                       const float64 & orf64_InteractionPointWidth) :
   //lint -e{155}  C++11 feature
   mc_Points(
{
   orc_Line.p1(), orc_Line.p2()
}
      ),
   mf64_Width(orf64_Width),
   mf64_InteractionPointWidth(orf64_InteractionPointWidth)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get shape for initialized object

   \return
   Valid shape

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
QPainterPath C_GiBiLineBounding::GetShape()
{
   QPainterPath c_Retval;
   sint32 s32_FirstPointInSecondSegment;

   this->mc_Bounding.clear();

   c_Retval.setFillRule(Qt::WindingFill);

   for (sint32 s32_It = 0; s32_It < static_cast<sint32>(this->mc_Points.size() - 1); ++s32_It)
   {
      this->AppendLineBoundingPointsTop(s32_It, s32_It + 1);
   }
   m_ConsolidateLine(0, static_cast<sint32>(this->mc_Bounding.size() - 1));
   s32_FirstPointInSecondSegment = this->mc_Bounding.size();
   for (sint32 s32_It = static_cast<sint32>(this->mc_Points.size() - 1); s32_It > 0; --s32_It)
   {
      this->AppendLineBoundingPointsTop(s32_It, s32_It - 1);
   }
   m_ConsolidateLine(s32_FirstPointInSecondSegment, static_cast<sint32>(this->mc_Bounding.size() - 1));
   //Finish
   if (this->mc_Bounding.size() > 0)
   {
      this->mc_Bounding.append(this->mc_Bounding.at(0));
   }
   c_Retval.addPolygon(this->mc_Bounding);
   //Start
   return c_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get vector with length of 1

   \param[in] orc_P Point to evaluate

   \return
   Input vetor with length of 1

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
QPointF C_GiBiLineBounding::GetNorm(const QPointF & orc_P)
{
   const float64 f64_Length = GetLength(orc_P);
   QPointF c_Retval;

   if (std::abs(f64_Length) > 0.0)
   {
      c_Retval = QPointF(orc_P.x() / f64_Length, orc_P.y() / f64_Length);
   }
   else
   {
      c_Retval = orc_P;
   }

   return c_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set input vector to length

   \param[in] orc_P        Point to evaluate
   \param[in] orf64_Length Length to adapt to

   \return
   Input vetor with length of width

   \created     28.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
QPointF C_GiBiLineBounding::AdaptVecToWidth(const QPointF & orc_P, const float64 & orf64_Length)
{
   const QPointF c_Retval = GetNorm(orc_P);

   return QPointF(c_Retval.x() * orf64_Length, c_Retval.y() * orf64_Length);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get vector perpendicular to original

   \param[in] orc_P vector to get perpendicular for

   \created     28.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
QPointF C_GiBiLineBounding::GetPerpendicular(const QPointF & orc_P)
{
   return QPointF(-orc_P.y(), orc_P.x());
}

//-----------------------------------------------------------------------------
/*!
   \brief   Standard cross product

   \param[in] orc_P1 First point
   \param[in] orc_P2 Second point

   \return
   Result

   \created     21.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
float64 C_GiBiLineBounding::h_CrossProduct(const QPointF & orc_P1, const QPointF & orc_P2)
{
   const float64 f64_Retval = (orc_P1.x() * orc_P2.y()) - (orc_P1.y() * orc_P2.x());

   return f64_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add two bounding points to line shape

   \param[in] ors32_IndexStart Index of line start in stored points
   \param[in] ors32_IndexEnd   Index of line end in stored points

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_GiBiLineBounding::AppendLineBoundingPointsTop(const sint32 & ors32_IndexStart, const sint32 & ors32_IndexEnd)
{
   if ((ors32_IndexStart < this->mc_Points.size()) && (ors32_IndexEnd < this->mc_Points.size()))
   {
      const QPointF c_LineVec = this->mc_Points[ors32_IndexEnd] - this->mc_Points[ors32_IndexStart];
      const QPointF c_LinePerpendicularVec = GetPerpendicular(c_LineVec);
      const QPointF c_AdaptedLineVec = AdaptVecToWidth(c_LineVec);
      const QPointF c_AdaptedLinePerpendicularVec = AdaptVecToWidth(c_LinePerpendicularVec);

      //Start
      this->mc_Bounding.append((this->mc_Points[ors32_IndexStart] - c_AdaptedLineVec) - c_AdaptedLinePerpendicularVec);
      //End
      this->mc_Bounding.append((this->mc_Points[ors32_IndexEnd] + c_AdaptedLineVec) - c_AdaptedLinePerpendicularVec);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set input vector to length of width

   \param[in] orc_P Point to evaluate

   \return
   Input vetor with length of width

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
QPointF C_GiBiLineBounding::AdaptVecToWidth(const QPointF & orc_P) const
{
   const float64 f64_Factor = (this->mf64_Width + this->mf64_InteractionPointWidth) / 2.0;

   return AdaptVecToWidth(orc_P, f64_Factor);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get length of vector

   \param[in] orc_P Point to evaluate

   \return
   Length of input vector

   \created     12.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
float64 C_GiBiLineBounding::GetLength(const QPointF & orc_P)
{
   const float64 f64_Retval = std::sqrt((orc_P.x() * orc_P.x()) + (orc_P.y() * orc_P.y()));

   return f64_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Consolidate intersection points of line segments in specified index range

   \param[in] ors32_RangeIndexStart Start index of range to consolidate
   \param[in] ors32_RangeIndexEnd   Last index of range to consolidate

   \created     24.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_GiBiLineBounding::m_ConsolidateLine(const sint32 & ors32_RangeIndexStart, const sint32 & ors32_RangeIndexEnd)
{
   //Check if at least two lines are possible
   if ((ors32_RangeIndexEnd - ors32_RangeIndexStart) >= 3)
   {
      QPointF c_Intersection;
      //for each line
      for (sint32 s32_ItCurLine = ors32_RangeIndexStart;
           (s32_ItCurLine <= ors32_RangeIndexEnd) &&
           ((s32_ItCurLine < static_cast<sint32>(this->mc_Bounding.size() - 1)) && (s32_ItCurLine >= 0));
           s32_ItCurLine += 2)
      {
         //for each other line
         for (sint32 s32_ItCounterLine = ors32_RangeIndexStart;
              ((s32_ItCounterLine <= ors32_RangeIndexEnd) && (s32_ItCounterLine != s32_ItCurLine)) &&
              ((s32_ItCounterLine < static_cast<sint32>(this->mc_Bounding.size() - 1)) && (s32_ItCounterLine >= 0));
              s32_ItCounterLine += 2)
         {
            //Check if intersection would affect too many points
            //(Not allowed as this may indicate crossing lines which should not affect the shape)
            if ((std::max(s32_ItCurLine, s32_ItCounterLine) - std::min(s32_ItCurLine, s32_ItCounterLine)) < 4)
            {
               if (m_CalcIntersection(this->mc_Bounding.at(s32_ItCurLine),
                                      this->mc_Bounding.at(static_cast<sintn>(s32_ItCurLine + 1L)),
                                      this->mc_Bounding.at(s32_ItCounterLine),
                                      this->mc_Bounding.at(static_cast<sintn>(s32_ItCounterLine + 1L)),
                                      c_Intersection) == true)
               {
                  //for all points in segment (exclude first and last point as those should not be affected)
                  for (sint32 s32_ItCurPoint = std::min(s32_ItCurLine, s32_ItCounterLine) + 1;
                       (s32_ItCurPoint <= std::max(s32_ItCurLine, s32_ItCounterLine)) &&
                       ((s32_ItCurPoint < this->mc_Bounding.size()) && (s32_ItCurPoint >= 0));
                       ++s32_ItCurPoint)
                  {
                     this->mc_Bounding[s32_ItCurPoint] = c_Intersection;
                  }
               }
            }
         }
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Calculate intersection point if possible

   See http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect @Gareth Rees

   \param[in]  orc_Start1       Start of line one
   \param[in]  orc_End1         End of line one
   \param[in]  orc_Start2       Start of line two
   \param[in]  orc_End2         End of line two
   \param[out] orc_Intersection Intersection point

   \return
   true:  Intersection found
   false: No intersection

   \created     21.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
bool C_GiBiLineBounding::m_CalcIntersection(const QPointF & orc_Start1, const QPointF & orc_End1,
                                            const QPointF & orc_Start2, const QPointF & orc_End2,
                                            QPointF & orc_Intersection)
{
   bool q_Retval;
   const QPointF c_Diff1 = orc_End1 - orc_Start1;
   const QPointF c_Diff2 = orc_End2 - orc_Start2;
   const float64 f64_Cross1 = h_CrossProduct(c_Diff1, c_Diff2);

   if (C_OSCUtils::h_IsFloat64NearlyEqual(f64_Cross1, 0.0) == true)
   {
      const float64 f64_Cross2 = h_CrossProduct(orc_Start2 - orc_Start1, c_Diff1);
      if (C_OSCUtils::h_IsFloat64NearlyEqual(f64_Cross2, 0.0) == true)
      {
         const float64 f64_T0 = QPointF::dotProduct((orc_Start2 - orc_Start1), c_Diff1) / QPointF::dotProduct(c_Diff1,
                                                                                                              c_Diff1);
         const float64 f64_T1 =
            QPointF::dotProduct(((orc_Start2 + c_Diff2) - orc_Start1), c_Diff1) / QPointF::dotProduct(
               c_Diff1, c_Diff1);
         if ((mq_Within(f64_T0, 0.0, 1.0) == true) && (mq_Within(f64_T1, 0.0, 1.0) == true))
         {
            q_Retval = true;
            orc_Intersection = orc_Start1 + (f64_T0 * c_Diff1);
         }
         else
         {
            q_Retval = false;
         }
      }
      else
      {
         q_Retval = false;
      }
   }
   else
   {
      const float64 f64_T =
         h_CrossProduct(orc_Start2 - orc_Start1, c_Diff2) / h_CrossProduct(c_Diff1, c_Diff2);
      const float64 f64_U =
         h_CrossProduct(orc_Start1 - orc_Start2, c_Diff1) / h_CrossProduct(c_Diff2, c_Diff1);
      if ((mq_Within(f64_T, 0.0, 1.0) == true) && (mq_Within(f64_U, 0.0, 1.0) == true))
      {
         orc_Intersection = orc_Start1 + (f64_T * c_Diff1);
         q_Retval = true;
      }
      else
      {
         q_Retval = false;
      }
   }
   return q_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Check if value within boundaries

   \param[in] orf64_Eval  Point to check for boundaries
   \param[in] orf64_Start Lower boundary
   \param[in] orf64_End   Upper boundary

   \return
   true:  Within boundaries
   false: Outside boundaries

   \created     21.10.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
bool C_GiBiLineBounding::mq_Within(const float64 & orf64_Eval, const float64 & orf64_Start, const float64 & orf64_End)
{
   bool q_Retval;

   if ((orf64_Eval >= orf64_Start) && (orf64_Eval <= orf64_End))
   {
      q_Retval = true;
   }
   else
   {
      q_Retval = false;
   }
   return q_Retval;
}