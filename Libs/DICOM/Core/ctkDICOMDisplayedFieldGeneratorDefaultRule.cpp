/*=========================================================================

  Library:   CTK

  Copyright (c) PerkLab 2018

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#include "ctkDICOMDisplayedFieldGeneratorDefaultRule.h"

// dcmtk includes
#include "dcmtk/dcmdata/dcvrpn.h"

//------------------------------------------------------------------------------
QStringList ctkDICOMDisplayedFieldGeneratorDefaultRule::getRequiredDICOMTags()
{
  QStringList requiredTags;

  requiredTags << dicomTagToString(DCM_SOPInstanceUID);

  requiredTags << dicomTagToString(DCM_PatientID);
  requiredTags << dicomTagToString(DCM_PatientName);
  requiredTags << dicomTagToString(DCM_PatientBirthDate);
  requiredTags << dicomTagToString(DCM_PatientBirthTime);
  requiredTags << dicomTagToString(DCM_PatientSex);
  requiredTags << dicomTagToString(DCM_PatientAge);
  requiredTags << dicomTagToString(DCM_PatientComments);

  requiredTags << dicomTagToString(DCM_StudyInstanceUID);
  requiredTags << dicomTagToString(DCM_StudyID);
  requiredTags << dicomTagToString(DCM_StudyDate);
  requiredTags << dicomTagToString(DCM_StudyTime);
  requiredTags << dicomTagToString(DCM_AccessionNumber);
  requiredTags << dicomTagToString(DCM_ModalitiesInStudy);
  requiredTags << dicomTagToString(DCM_InstitutionName);
  requiredTags << dicomTagToString(DCM_PerformingPhysicianName);
  requiredTags << dicomTagToString(DCM_ReferringPhysicianName);
  requiredTags << dicomTagToString(DCM_StudyDescription);

  requiredTags << dicomTagToString(DCM_SeriesInstanceUID);
  requiredTags << dicomTagToString(DCM_SeriesDate);
  requiredTags << dicomTagToString(DCM_SeriesTime);
  requiredTags << dicomTagToString(DCM_SeriesDescription);
  requiredTags << dicomTagToString(DCM_Modality);
  requiredTags << dicomTagToString(DCM_BodyPartExamined);
  requiredTags << dicomTagToString(DCM_FrameOfReferenceUID);
  requiredTags << dicomTagToString(DCM_ContrastBolusAgent);
  requiredTags << dicomTagToString(DCM_ScanningSequence);
  requiredTags << dicomTagToString(DCM_SeriesNumber);
  requiredTags << dicomTagToString(DCM_AcquisitionNumber);
  requiredTags << dicomTagToString(DCM_EchoNumbers);
  requiredTags << dicomTagToString(DCM_TemporalPositionIdentifier);   
  requiredTags << dicomTagToString(DCM_Rows);
  requiredTags << dicomTagToString(DCM_Columns);

  return requiredTags;
}

//------------------------------------------------------------------------------
void ctkDICOMDisplayedFieldGeneratorDefaultRule::registerEmptyFieldNames(
  QMap<QString, QString> emptyFieldsDisplaySeries,
  QMap<QString, QString> emptyFieldsDisplayStudies,
  QMap<QString, QString> emptyFieldsDisplayPatients )
{
  Q_UNUSED(emptyFieldsDisplaySeries);
  Q_UNUSED(emptyFieldsDisplayStudies);
  Q_UNUSED(emptyFieldsDisplayPatients);
  emptyFieldsDisplaySeries.insertMulti("SeriesDescription", EMPTY_SERIES_DESCRIPTION);
}

//------------------------------------------------------------------------------
void ctkDICOMDisplayedFieldGeneratorDefaultRule::getDisplayedFieldsForInstance(
  const QMap<QString, QString> &cachedTagsForInstance, QMap<QString, QString> &displayedFieldsForCurrentSeries,
  QMap<QString, QString> &displayedFieldsForCurrentStudy, QMap<QString, QString> &displayedFieldsForCurrentPatient )
{
  displayedFieldsForCurrentPatient["PatientsName"] = cachedTagsForInstance[dicomTagToString(DCM_PatientName)];
  displayedFieldsForCurrentPatient["PatientID"] = cachedTagsForInstance[dicomTagToString(DCM_PatientID)];
  displayedFieldsForCurrentPatient["DisplayedPatientsName"] = humanReadablePatientName(cachedTagsForInstance[dicomTagToString(DCM_PatientName)]);

  displayedFieldsForCurrentStudy["StudyInstanceUID"] = cachedTagsForInstance[dicomTagToString(DCM_StudyInstanceUID)];
  displayedFieldsForCurrentStudy["PatientIndex"] = displayedFieldsForCurrentPatient["PatientIndex"];
  displayedFieldsForCurrentStudy["StudyDescription"] = cachedTagsForInstance[dicomTagToString(DCM_StudyDescription)];
  displayedFieldsForCurrentStudy["StudyDate"] = cachedTagsForInstance[dicomTagToString(DCM_StudyDate)];
  displayedFieldsForCurrentStudy["ModalitiesInStudy"] = cachedTagsForInstance[dicomTagToString(DCM_ModalitiesInStudy)];
  displayedFieldsForCurrentStudy["InstitutionName"] = cachedTagsForInstance[dicomTagToString(DCM_InstitutionName)];
  displayedFieldsForCurrentStudy["ReferringPhysician"] = cachedTagsForInstance[dicomTagToString(DCM_ReferringPhysicianName)];

  displayedFieldsForCurrentSeries["SeriesInstanceUID"] = cachedTagsForInstance[dicomTagToString(DCM_SeriesInstanceUID)];
  displayedFieldsForCurrentSeries["StudyInstanceUID"] = cachedTagsForInstance[dicomTagToString(DCM_StudyInstanceUID)];
  displayedFieldsForCurrentSeries["SeriesNumber"] = cachedTagsForInstance[dicomTagToString(DCM_SeriesNumber)];
  displayedFieldsForCurrentSeries["Modality"] = cachedTagsForInstance[dicomTagToString(DCM_Modality)];
  displayedFieldsForCurrentSeries["SeriesDescription"] = cachedTagsForInstance[dicomTagToString(DCM_SeriesDescription)];
  if ( cachedTagsForInstance.contains(dicomTagToString(DCM_Rows)) && !cachedTagsForInstance[dicomTagToString(DCM_Rows)].isEmpty()
    && cachedTagsForInstance.contains(dicomTagToString(DCM_Columns)) && !cachedTagsForInstance[dicomTagToString(DCM_Columns)].isEmpty() )
  {
    QString rows = cachedTagsForInstance[dicomTagToString(DCM_Rows)];
    QString columns = cachedTagsForInstance[dicomTagToString(DCM_Columns)];
    displayedFieldsForCurrentSeries["DisplayedSize"] = QString("%1x%2").arg(columns).arg(rows);
  }
}

//------------------------------------------------------------------------------
void ctkDICOMDisplayedFieldGeneratorDefaultRule::mergeDisplayedFieldsForInstance(
  const QMap<QString, QString> &initialFieldsSeries, const QMap<QString, QString> &initialFieldsStudy, const QMap<QString, QString> &initialFieldsPatient,
  const QMap<QString, QString> &newFieldsSeries, const QMap<QString, QString> &newFieldsStudy, const QMap<QString, QString> &newFieldsPatient,
  QMap<QString, QString> &mergedFieldsSeries, QMap<QString, QString> &mergedFieldsStudy, QMap<QString, QString> &mergedFieldsPatient,
  const QMap<QString, QString> &emptyFieldsSeries, const QMap<QString, QString> &emptyFieldsStudy, const QMap<QString, QString> &emptyFieldsPatient
  )
{
  mergeExpectSameValue("PatientIndex",          initialFieldsPatient, newFieldsPatient, mergedFieldsPatient, emptyFieldsPatient);
  mergeExpectSameValue("PatientsName",          initialFieldsPatient, newFieldsPatient, mergedFieldsPatient, emptyFieldsPatient);
  mergeExpectSameValue("PatientID",             initialFieldsPatient, newFieldsPatient, mergedFieldsPatient, emptyFieldsPatient);
  mergeExpectSameValue("DisplayedPatientsName", initialFieldsPatient, newFieldsPatient, mergedFieldsPatient, emptyFieldsPatient);

  mergeExpectSameValue("StudyInstanceUID",   initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);
  mergeExpectSameValue("PatientIndex",       initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);
  mergeConcatenate    ("StudyDescription",   initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);
  mergeExpectSameValue("StudyDate",          initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);
  mergeConcatenate    ("ModalitiesInStudy",  initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);
  mergeExpectSameValue("InstitutionName",    initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);
  mergeConcatenate    ("ReferringPhysician", initialFieldsStudy, newFieldsStudy, mergedFieldsStudy, emptyFieldsStudy);

  mergeExpectSameValue("SeriesInstanceUID",  initialFieldsSeries, newFieldsSeries, mergedFieldsSeries, emptyFieldsSeries);
  mergeExpectSameValue("StudyInstanceUID",   initialFieldsSeries, newFieldsSeries, mergedFieldsSeries, emptyFieldsSeries);
  mergeExpectSameValue("SeriesNumber",       initialFieldsSeries, newFieldsSeries, mergedFieldsSeries, emptyFieldsSeries);
  mergeExpectSameValue("Modality",           initialFieldsSeries, newFieldsSeries, mergedFieldsSeries, emptyFieldsSeries);
  mergeConcatenate    ("SeriesDescription",  initialFieldsSeries, newFieldsSeries, mergedFieldsSeries, emptyFieldsSeries);
  mergeExpectSameValue("DisplayedSize", initialFieldsSeries, newFieldsSeries, mergedFieldsSeries, emptyFieldsSeries);
}

//------------------------------------------------------------------------------
QString ctkDICOMDisplayedFieldGeneratorDefaultRule::humanReadablePatientName(const QString & dicomPatientName)
{
  QString formattedName("");
  const QStringList d = dicomPatientName.split(QString("="), QString::SkipEmptyParts);
  for (int x = 0; x < d.size(); x++)
  {
    if (!formattedName.isEmpty()) formattedName += QString(" = ");
    // Last, First, Middle, Prefix, Suffix.
    const QStringList l = d.at(x).split(QString("^"), QString::KeepEmptyParts);
    const int k = l.size();
    const QString lastName   = k > 0 ? l.at(0).trimmed().remove(QChar('\0')) : "";
    const QString firstName  = k > 1 ? l.at(1).trimmed().remove(QChar('\0')) : "";
    const QString middleName = k > 2 ? l.at(2).trimmed().remove(QChar('\0')) : "";
    const QString namePrefix = k > 3 ? l.at(3).trimmed().remove(QChar('\0')) : "";
    const QString nameSuffix = k > 4 ? l.at(4).trimmed().remove(QChar('\0')) : "";
    // Concatenate name components per this convention: Last, First Middle, Suffix (Prefix).
    if (!lastName.isEmpty())
    {
      formattedName += lastName;
      if (!(firstName.isEmpty() && middleName.isEmpty()))
      {
        formattedName += ",";
      }
    }
    if (!firstName.isEmpty())
    {
      formattedName += " ";
      formattedName += firstName;
    }
    if (!middleName.isEmpty())
    {
      formattedName += " ";
      formattedName += middleName;
    }
    if (!nameSuffix.isEmpty())
    {
      formattedName += ", ";
      formattedName += nameSuffix;
    }
    if (!namePrefix.isEmpty())
    {
      formattedName += " (";
      formattedName += namePrefix;
      formattedName += ")";
    }
#if 0
    // For backward compatibility, read 1st "=" block only.
    break;
#endif
  }
  return formattedName;
}
