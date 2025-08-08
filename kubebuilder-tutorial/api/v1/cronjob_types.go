package v1

import (
	batchv1 "k8s.io/api/batch/v1"
	corev1 "k8s.io/api/core/v1"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
)

type CronJobSpec struct {
	// +kubebuilder:validation:MinLength=0
	// +required
	Schedule string `json:"schedule"`

	// +optional
	// +kubebuilder:validation:Minimum=0
	StartingDeadlineSeconds *int64 `json:"startingDeadlineSeconds,omitempty"`

	// +optional
	ConcurrencyPolicy ConcurrencyPolicy `json:"concurrencyPolicy,omitempty"`

	// +optional
	Suspend *bool `json:"suspend,omitempty"`

	JobTemplate batchv1.JobTemplateSpec `json:"jobTemplate"`

	// +optional
	// +kubebuilder:validation:Minimum=0
	SuccessfulJobsHistoryLimit *int32 `json:"successfulJobsHistoryLimit,omitempty"`

	// +optional
	// +kubebuilder:validation:Minimum=0
	FailedJobsHistoryLimit *int32 `json:"failedJobsHistoryLimit,omitempty"`
}

type ConcurrencyPolicy string

const (
	AllowConcurrent   ConcurrencyPolicy = "Allow"
	ForbidConcurrent  ConcurrencyPolicy = "Forbid"
	ReplaceConcurrent ConcurrencyPolicy = "Replace"
)

type CronJobStatus struct {
	// +optional
	Active []corev1.ObjectReference `json:"active,omitempty"`

	// +optional
	LastScheduleTime *metav1.Time `json:"lastScheduleTime,omitempty"`
}

// +kubebuilder:object:root=true
// +kubebuilder:subresource:status

type CronJob struct {
	metav1.TypeMeta `json:",inline"`

	// +optional
	metav1.ObjectMeta `json:"metadata,omitempty,omitzero"`

	// +required
	Spec CronJobSpec `json:"spec"`

	// +optional
	Status CronJobStatus `json:"status,omitempty,omitzero"`
}

// +kubebuilder:object:root=true

type CronJobList struct {
	metav1.TypeMeta `json:",inline"`
	metav1.ListMeta `json:"metadata,omitempty"`
	Items           []CronJob `json:"items"`
}

func init() {
	SchemeBuilder.Register(&CronJob{}, &CronJobList{})
}
